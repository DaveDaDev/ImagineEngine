#include "ielog.h"

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static void* _clog_logger_process(void* arg);
static int   _clog_logger_process_running(ClogLogger* p_logger_handle);
static void  _clog_sink_on_message(ClogSink* p_sink_handle, ClogMessage* p_message_handle);


ClogLogger* clog_logger_create(void)
{
    ClogLogger* p_logger_handle = (ClogLogger*) malloc(sizeof(ClogLogger)); 

    p_logger_handle->message_buffer_read_index = 0;
    p_logger_handle->message_buffer_write_index = 0;

    p_logger_handle->sinks = NULL;

    p_logger_handle->message_buffer = (ClogMessage*) malloc(sizeof(ClogMessage)
            * CLOG_LOGGER_MESSAGE_BUFFER_DEFAULT_SIZE);

    sem_init(&p_logger_handle->semaphore_message_buffer_read, CLOG_SEMIPHORE_SINGLE_PROCESS, 0);
    sem_init(&p_logger_handle->semaphore_message_buffer_write, CLOG_SEMIPHORE_SINGLE_PROCESS,
           CLOG_LOGGER_MESSAGE_BUFFER_DEFAULT_SIZE); 

    pthread_mutex_init(&p_logger_handle->mutex_message_buffer_write_index, NULL);
    pthread_mutex_init(&p_logger_handle->mutex_sinks, NULL);

    pthread_mutex_init(&p_logger_handle->mutex_process, NULL);
    pthread_mutex_lock(&p_logger_handle->mutex_process);
    pthread_create(&p_logger_handle->thread_handle, NULL, _clog_logger_process, p_logger_handle); 

    return p_logger_handle;
}


static void* _clog_logger_process(void* arg)
{
    ClogLogger* p_logger_handle = (ClogLogger*) arg; 

    while(_clog_logger_process_running(p_logger_handle)) {

        sem_wait(&p_logger_handle->semaphore_message_buffer_read);

        p_logger_handle->message_buffer_read_index++;
        if (p_logger_handle->message_buffer_read_index > CLOG_LOGGER_MESSAGE_BUFFER_DEFAULT_SIZE)
            p_logger_handle->message_buffer_read_index = 0;

        ClogMessage* p_message_handle = (ClogMessage*) p_logger_handle->message_buffer
                + p_logger_handle->message_buffer_read_index;
        
        pthread_mutex_lock(&p_logger_handle->mutex_sinks);        

        ClogSink* p_sink_iterator = p_logger_handle->sinks; 
        while(p_sink_iterator != NULL) {
            _clog_sink_on_message(p_sink_iterator, p_message_handle);
            p_sink_iterator = p_sink_iterator->next_sink;
        }

        pthread_mutex_unlock(&p_logger_handle->mutex_sinks);

        sem_post(&p_logger_handle->semaphore_message_buffer_write);

    }

    return NULL;
}


static int _clog_logger_process_running(ClogLogger* p_logger_handle)
{
    int messages_remaining;
    int logger_processing;
    sem_getvalue(&p_logger_handle->semaphore_message_buffer_read, &messages_remaining);

    logger_processing = pthread_mutex_trylock(&p_logger_handle->mutex_process);
    if (logger_processing == 0)
        pthread_mutex_unlock(&p_logger_handle->mutex_process);

    return (logger_processing == 0) && (messages_remaining == 0) ? 0 : 1; 
}


void clog_logger_destroy(ClogLogger* p_logger_handle)
{
    pthread_mutex_unlock(&p_logger_handle->mutex_process);
    clog_message_dispatch(p_logger_handle, 0, 1, __LINE__, __FILE__, "---End of Log---");
    pthread_join(p_logger_handle->thread_handle, NULL);
   
    sem_destroy(&p_logger_handle->semaphore_message_buffer_read);
    sem_destroy(&p_logger_handle->semaphore_message_buffer_write);

    pthread_mutex_destroy(&p_logger_handle->mutex_message_buffer_write_index);
    pthread_mutex_destroy(&p_logger_handle->mutex_sinks);
    pthread_mutex_destroy(&p_logger_handle->mutex_process);

    clog_logger_destory_attached_sinks(p_logger_handle);

    free(p_logger_handle->message_buffer);
    free(p_logger_handle);
}


void clog_logger_destory_attached_sinks(ClogLogger* p_logger_handle)
{
    ClogSink* p_sink_iterator = p_logger_handle->sinks;
    ClogSink* p_sink_tmp;
    while(p_sink_iterator != NULL) {
       p_sink_tmp = p_sink_iterator->next_sink; 
       clog_logger_detach_sink(p_logger_handle, p_sink_iterator);
       clog_sink_destroy(p_sink_iterator);
       p_sink_iterator = p_sink_tmp;
    }
}


void clog_logger_attach_sink(ClogLogger* p_logger_handle, ClogSink* p_sink_handle)
{

    if (p_logger_handle->sinks == NULL) {
        p_logger_handle->sinks = p_sink_handle;
        return;
    }

    ClogSink* p_sink_iterator = p_logger_handle->sinks;
    while(p_sink_iterator->next_sink != NULL) {
        p_sink_iterator = p_sink_iterator->next_sink; 
    }
    p_sink_iterator->next_sink = p_sink_handle;

}


void clog_logger_detach_sink(ClogLogger* p_logger_handle, ClogSink* p_sink_handle)
{
    pthread_mutex_lock(&p_logger_handle->mutex_sinks);

    ClogSink* p_sink_iterator = p_logger_handle->sinks;
    ClogSink* p_sink_previous = p_logger_handle->sinks;
    while(p_sink_iterator != p_sink_handle) {
        p_sink_previous = p_sink_iterator;
        p_sink_iterator = p_sink_iterator->next_sink;
    } 
    p_sink_previous->next_sink = p_sink_iterator->next_sink;

    pthread_mutex_unlock(&p_logger_handle->mutex_sinks);
}

ClogSink* clog_sink_create(void (*callback_on_create)(ClogSink*),
                           void (*callback_on_message)(ClogSink*, ClogMessage*),
                           void (*callback_on_destroy)(ClogSink*))
{
    ClogSink* p_sink_handle = malloc(sizeof(ClogSink)); 

    p_sink_handle->next_sink = NULL;
    p_sink_handle->verbosity = CLOG_SINK_DEFAULT_VERBOSITY;
    p_sink_handle->channels = CLOG_SINK_DEFAULT_CHANNEL_FLAGS;
    p_sink_handle->callback_on_message = callback_on_message;
    p_sink_handle->callback_on_destroy = callback_on_destroy;

    if (callback_on_create != NULL)
        callback_on_create(p_sink_handle);

    return p_sink_handle;
}


void clog_sink_destroy(ClogSink* p_sink_handle)
{
    if (p_sink_handle->callback_on_destroy != NULL)
       p_sink_handle->callback_on_destroy(p_sink_handle); 

    free(p_sink_handle);
}


void clog_sink_set_verbosity(ClogSink* p_sink_handle, const int new_verbosity)
{
    p_sink_handle->verbosity = new_verbosity;
}


void clog_sink_set_channels(ClogSink* p_sink_handle, const unsigned int new_channel_flags)
{
    p_sink_handle->channels = new_channel_flags;
}


static void _clog_sink_on_message(ClogSink* p_sink_handle, ClogMessage* p_message_handle)
{
    if (p_message_handle->verbosity < p_sink_handle->verbosity)
        return;

    if ((p_message_handle->channels & p_sink_handle->channels) == 0x0)
        return;

    if (p_sink_handle->callback_on_message != NULL)
        p_sink_handle->callback_on_message(p_sink_handle, p_message_handle);
}


void clog_message_dispatch(ClogLogger* p_logger_handle,
                           const int verbosity,
                           const unsigned int channel_flags,
                           const unsigned int fileline,
                           const char* filename,
                           const char* message_pattern, ...)
{
    if (p_logger_handle == NULL)
        return;


    struct timeval new_timestamp;
    gettimeofday(&new_timestamp, NULL);

    sem_wait(&p_logger_handle->semaphore_message_buffer_write);

    pthread_mutex_lock(&p_logger_handle->mutex_message_buffer_write_index);

    p_logger_handle->message_buffer_write_index++;
    if (p_logger_handle->message_buffer_write_index > CLOG_LOGGER_MESSAGE_BUFFER_DEFAULT_SIZE)
        p_logger_handle->message_buffer_write_index = 0;

    ClogMessage* p_message_handle = (ClogMessage*) p_logger_handle->message_buffer
            + p_logger_handle->message_buffer_write_index; 

    pthread_mutex_unlock(&p_logger_handle->mutex_message_buffer_write_index);
    
    p_message_handle->verbosity = verbosity;
    p_message_handle->channels = channel_flags;
    p_message_handle->timestamp = new_timestamp;
    p_message_handle->fileline = fileline;
    snprintf(p_message_handle->filename, CLOG_MESSAGE_FILENAME_BUFFER_BYTES, filename);

    va_list vargs;
    va_start(vargs, message_pattern);
    vsnprintf(p_message_handle->message, CLOG_MESSAGE_STRING_BUFFER_BYTES, message_pattern, vargs);
    va_end(vargs);

    sem_post(&p_logger_handle->semaphore_message_buffer_read);
}
