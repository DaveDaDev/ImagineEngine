#include "iecore.h"

#include <stdio.h>


void ie_core_startup(void)
{
    iec_debug_memory_startup();
    ie_core_log_startup();
    ie_core_window_startup();
    ie_core_event_startup();
}


void ie_core_shutdown(void)
{
    ie_core_event_shutdown();
    ie_core_window_shutdown();
    ie_core_log_shutdown();
    iec_debug_memory_shutdown();
}
