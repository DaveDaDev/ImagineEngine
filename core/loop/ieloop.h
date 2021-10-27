#ifndef IE_CORE_LOOP
#define IE_CORE_LOOP

#include <inttypes.h>

typedef struct {
    char name[255];
    void (*procedure_startup) ();
    void (*procedure_update) ();
    void (*procedure_shutdown) ();
} IEModule;

typedef struct {
    IEModule* modules;
    uint32_t* sequence_startup;
    uint32_t* sequence_update;
    uint32_t* sequence_shutdown;
    uint32_t  count_module;
    uint32_t  sequence_count_startup;
    uint32_t  sequence_count_update;
    uint32_t  sequence_count_shutdown;
    int       running;
} IELoop;

void ie_engine_run(IELoop* loop);
void ie_engine_stop(IELoop* loop);
void ie_engine_sequence_startup(IELoop* loop);
void ie_engine_sequence_update(IELoop* loop);
void ie_engine_sequence_shutdown(IELoop* loop);
int  ie_engine_is_running(IELoop* loop);

#endif
