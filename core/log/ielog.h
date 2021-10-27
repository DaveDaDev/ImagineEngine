#ifndef  IE_CORE_LOG_H
#define  IE_CORE_LOG_H 

#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#define CLOG_LOGGER_MESSAGE_BUFFER_DEFAULT_SIZE 1024 
#define CLOG_SINK_DEFAULT_VERBOSITY 0
#define CLOG_SINK_DEFAULT_CHANNEL_FLAGS 0xFFFFFFFF
#define CLOG_MESSAGE_STRING_BUFFER_BYTES 512 
#define CLOG_MESSAGE_FILENAME_BUFFER_BYTES 128
#define CLOG_SEMIPHORE_SINGLE_PROCESS 0

/* Clog Core */

typedef struct {
    unsigned int   channels;
    unsigned int   fileline;
    int            verbosity;
    struct timeval timestamp;
    char           filename[CLOG_MESSAGE_FILENAME_BUFFER_BYTES];
    char           message[CLOG_MESSAGE_STRING_BUFFER_BYTES];
} ClogMessage;

typedef struct ClogSink {
    struct       ClogSink* next_sink;
    int          verbosity;
    unsigned int channels;
    void         (*callback_on_message)(struct ClogSink*, ClogMessage*);
    void         (*callback_on_destroy)(struct ClogSink*); 
    void*        extention;
} ClogSink;

typedef struct {
    unsigned int    message_buffer_read_index;
    unsigned int    message_buffer_write_index;
    pthread_t       thread_handle;
    pthread_mutex_t mutex_process;
    pthread_mutex_t mutex_sinks;
    pthread_mutex_t mutex_message_buffer_write_index;
    sem_t           semaphore_message_buffer_write;
    sem_t           semaphore_message_buffer_read;
    ClogSink*       sinks; 
    ClogMessage*    message_buffer;
} ClogLogger;


extern ClogLogger* clog_logger_create(void);
extern void        clog_logger_destroy(ClogLogger* p_logger_handle);
extern void        clog_logger_destory_attached_sinks(ClogLogger* p_logger_handle);
extern void        clog_logger_attach_sink(ClogLogger* p_logger_handle, ClogSink* p_sink_handle);
extern void        clog_logger_detach_sink(ClogLogger* p_logger_handle, ClogSink* p_sink_handle);


extern ClogSink*   clog_sink_create(void (*callback_create)(ClogSink*),
                                    void (*callback_process)(ClogSink*, ClogMessage*),
                                    void (*callback_destroy)(ClogSink*));
extern void        clog_sink_destroy(ClogSink* p_sink_handle);
extern void        clog_sink_set_verbosity(ClogSink* p_sink_handle, const int new_verbosity);
extern void        clog_sink_set_channels(ClogSink* p_sink_handle, const unsigned int channel_flags);


extern void clog_message_dispatch(ClogLogger* p_logger_handle,
                                  const int verbosity,
                                  const unsigned int channel_flags,
                                  const unsigned int fileline,
                                  const char* filename,
                                  const char* message_pattern, ...);

/* Clog Quick Logger */

extern ClogLogger* p_clog_logger_quick_handle;

typedef enum {
    CLOG_TRACE = -2,
    CLOG_DEBUG = -1,
    CLOG_INFO = 0,
    CLOG_WARNING = 1,
    CLOG_ERROR = 2,
    CLOG_FATAL = 3
} ClogVerbosityLevel;

typedef enum {
    CLOG_CHANNEL_GENERAL = 0x1
} ClogChannels;

#ifdef CLOG_MESSAGE_DISPATCH_STRIP

#define TRACE(pattern, ...)
#define DEBUG(pattern, ...)
#define INFO(pattern, ...)
#define WARNING(pattern, ...)
#define ERROR(pattern, ...)
#define FATAL(pattern, ...)
#define LOG(pattern, ...)

#else

#define TRACE(pattern, ...)   clog_message_dispatch(p_clog_logger_quick_handle, CLOG_TRACE, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__); 
#define DEBUG(pattern, ...)   clog_message_dispatch(p_clog_logger_quick_handle, CLOG_DEBUG, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__);
#define INFO(pattern, ...)    clog_message_dispatch(p_clog_logger_quick_handle, CLOG_INFO, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__);
#define WARNING(pattern, ...) clog_message_dispatch(p_clog_logger_quick_handle, CLOG_WARNING, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__);
#define ERROR(pattern, ...)   clog_message_dispatch(p_clog_logger_quick_handle, CLOG_ERROR, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__);
#define FATAL(pattern, ...)   clog_message_dispatch(p_clog_logger_quick_handle, CLOG_FATAL, CLOG_CHANNEL_GENERAL, __LINE__, __FILE__, pattern, ##__VA_ARGS__);

#define LOG(verbosity, channel, pattern, ...) clog_message_dispatch(p_clog_logger_quick_handle, verbosity, channel, __LINE__, __FILE__, pattern, ##__VA_ARGS__);

#endif

void clog_logger_quick_create(void);
void clog_logger_quick_destroy(void);
void ie_core_log_startup(void);
void ie_core_log_shutdown(void);

#endif

