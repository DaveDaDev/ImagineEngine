#include "ielog.h"

#include <stdio.h>
#include <time.h>

ClogLogger* p_clog_logger_quick_handle = NULL;

static void _clog_sink_quick_console_process(ClogSink* p_sink_handle, ClogMessage* p_message_handle);

void clog_logger_quick_create(void)
{
    p_clog_logger_quick_handle = clog_logger_create();
    ClogSink* p_sink_handle = clog_sink_create(NULL, _clog_sink_quick_console_process, NULL);
    clog_sink_set_verbosity(p_sink_handle, CLOG_TRACE);
    clog_logger_attach_sink(p_clog_logger_quick_handle, p_sink_handle);
    clog_message_dispatch(p_clog_logger_quick_handle, CLOG_INFO, CLOG_CHANNEL_GENERAL,__LINE__, __FILE__, "---Start of Log---");
}


void clog_logger_quick_destroy(void)
{
    clog_logger_destroy(p_clog_logger_quick_handle);
}


static void _clog_sink_quick_console_process(ClogSink* p_sink_handle, ClogMessage* p_message_handle)
{
    
    struct tm* time_bd = gmtime(&((p_message_handle->timestamp).tv_sec));

    switch(p_message_handle->verbosity) {

    case CLOG_TRACE:
        fprintf(stdout, "GMT %d:%d:%d TRACE - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    case CLOG_DEBUG:
        fprintf(stdout, "GMT %d:%d:%d DEBUG - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    case CLOG_INFO:
        fprintf(stdout, "GMT %d:%d:%d INFO - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    case CLOG_WARNING:
        fprintf(stderr, "GMT %d:%d:%d \033[32mWARNING\033[m - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    case CLOG_ERROR:
        fprintf(stderr, "GMT %d:%d:%d \033[33mERROR\033[m - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    case CLOG_FATAL:
        fprintf(stderr, "GMT %d:%d:%d \033[31mFATAL\033[m - %s\n", time_bd->tm_hour, time_bd->tm_min, time_bd->tm_sec, p_message_handle->message); 
        break;
    }
}


void ie_core_log_startup()
{
    clog_logger_quick_create();
}


void ie_core_log_shutdown()
{
    clog_logger_quick_destroy();
}
