#include <stdio.h>
#include <stdlib.h>

#include "iedebugmemory.h"

void main()
{
    iec_debug_memory_startup();
    int i;
    for (i = 0; i < 10; ++i)
        malloc(sizeof(float));
    iec_debug_memory_print();
    
    void* ptest = malloc(sizeof(int));
    iec_debug_memory_print();
    //realloc(ptest, sizeof(int) * 2);
    iec_debug_memory_print();
    //free(ptest);
    iec_debug_memory_print();
   
    iec_debug_memory_shutdown();
}
