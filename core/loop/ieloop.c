#include "ieloop.h"

#include "core/log/ielog.h"

#include <inttypes.h>

void ie_engine_run(IELoop* loop)
{
    loop->running = 1;
    ie_engine_sequence_startup(loop);

    INFO("Engine Running...")

    while(ie_engine_is_running(loop)) {
        ie_engine_sequence_update(loop);
    }
    ie_engine_sequence_shutdown(loop);
}


void ie_engine_stop(IELoop* loop)
{
    loop->running = 0;
}


void ie_engine_sequence_startup(IELoop* loop)
{
   uint32_t i;
   for(i = 0; i < loop->sequence_count_startup; ++i) {
       uint32_t mid = loop->sequence_startup[i];
       IEModule* module = &loop->modules[mid];
       module->procedure_startup();
   }
}


void ie_engine_sequence_update(IELoop* loop)
{
   uint32_t i;
   for(i = 0; i < loop->sequence_count_update; ++i) {
       uint32_t mid = loop->sequence_update[i];
       IEModule* module = &loop->modules[mid];
       module->procedure_update();
   }
}


void ie_engine_sequence_shutdown(IELoop* loop)
{
   uint32_t i;
   for(i = 0; i < loop->sequence_count_shutdown; ++i) {
       uint32_t mid = loop->sequence_shutdown[i];
       IEModule* module = &loop->modules[mid];
       module->procedure_shutdown();
   }
}


int ie_engine_is_running(IELoop* loop)
{
    return loop->running;
}
