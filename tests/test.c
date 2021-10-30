#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/iecore.h"

IELoop game = {};
ie_window* window;

void mymodule_startup(void)
{
    INFO("My Module: Startup Procedure");
    window = ie_window_create(0 ,0, 1024, 768, 10, "Imagine Engine");
    FILE* config_file = ie_config_read("config");
    if (config_file == NULL)
        ERROR("Failed to open config file")
 
    ie_config_section_key_find(config_file, "start", "vector");
    char* name = ie_config_key_name_get(config_file);
    ie_config_result* result = ie_config_result_get(config_file, IE_ARRAY_STRING);
    if (ie_config_result_ok(result)) {
        int elements = result->size / sizeof(char*);
        char** data = (char**) result->pdata;
        INFO("Key: (%s), Elements: (%i), Value[0]: (%s)", name, elements, data[0])
    }
    free(name);
    ie_config_result_free(result);

    ie_config_close(config_file);
}


void mymodule_draw(void)
{
    ie_core_draw_rectangle(window, 100, 100, 100, 100);
}


void mymodule_update(void)
{
    ie_event_generic* ie_event;
    while( (ie_event = ie_core_event_get()) ) {
        switch (ie_event->type) {
            case IE_EVENT_WINDOW:
                ie_event_window* window_event = (ie_event_window*) ie_event;
                if (window_event->window_type == IE_EVENT_WINDOW_CLOSE) {
                    INFO("Engine Stop")
                    ie_engine_stop(&game);
                }
                if (window_event->window_type == IE_EVENT_WINDOW_EXPOSE) {
                    mymodule_draw();
                }
            break;

            case IE_EVENT_WINDOW_MOUSE:
            break;

            case IE_EVENT_MOUSE_BUTTON:
            break;

            case IE_EVENT_KEY:
            break;
        }
        free(ie_event);
    }
}


void mymodule_shutdown(void)
{
    INFO("My Module: Shutdown")
    free(window); 
}

int main()
{
    ie_core_startup();    

    INFO("Building Modules")
    IEModule mymodule = {};
    strcpy(mymodule.name, "My Module");
    mymodule.procedure_startup = mymodule_startup; 
    mymodule.procedure_update = mymodule_update;
    mymodule.procedure_shutdown = mymodule_shutdown;

    IEModule mymodules[1] = {mymodule};
    uint32_t my_sequence_startup[1] = {0};
    uint32_t my_sequence_update[1] = {0};
    uint32_t my_sequence_shutdown[1] = {0};

    INFO("Building Loop")
    game.modules = mymodules;
    game.count_module = 1;
    game.sequence_startup = my_sequence_startup;
    game.sequence_update = my_sequence_update;
    game.sequence_shutdown = my_sequence_shutdown;
    game.sequence_count_startup = 1;
    game.sequence_count_update = 1;
    game.sequence_count_shutdown = 1;

    ie_engine_run(&game);

    ie_core_shutdown();
}
