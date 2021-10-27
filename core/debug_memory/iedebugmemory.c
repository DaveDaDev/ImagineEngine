#include "../array_dynamic/iearraydynamic.h"

#include <stdio.h>

#include "iedebugmemory.h"

#ifdef IE_CORE_DEBUG_MEMORY_STATUS_ON

#undef malloc
#undef realloc 
#undef free 

#endif


iec_array_dynamic* _ie_core_debug_memory_dynamicarray = NULL;

void iec_debug_memory_startup()
{
   _ie_core_debug_memory_dynamicarray = iec_array_dynamic_create(sizeof(ie_debug_memory_allocation_record),
                                        IEC_ARRAY_DYNAMIC_DEFAULT_ELEMENT_AMOUNT,
                                        IEC_ARRAY_DYNAMIC_DEFAULT_GROWTH_RATIO);
}


void iec_debug_memory_shutdown()
{
    #ifdef IE_CORE_DEBUG_MEMORY_STATUS_ON
    iec_debug_memory_print();
    #endif

    iec_array_dynamic_destroy(_ie_core_debug_memory_dynamicarray);
}


void* iec_debug_memory_malloc(size_t size, char* file, unsigned int line)
{
    if (_ie_core_debug_memory_dynamicarray == NULL)
        return malloc(size);
    

    void* new_pointer = malloc(size);
    ie_debug_memory_allocation_record new_record;
    new_record.line = line;
    new_record.size = size;
    new_record.pdata = new_pointer; 

    int i;
    for (i = 0; i < 255 && file[i] != 0; ++i)
        new_record.file[i] = file[i];
    
    printf(">>Malloc (%p) - File: (%s) Line: (%u) Size (%u)\n", new_pointer, file, line, size); 
    iec_array_dynamic_push_back(_ie_core_debug_memory_dynamicarray, (void*)&new_record);

    return new_pointer; 
}


void* iec_debug_memory_realloc(void* p_old, size_t size, char* file, unsigned int line)
{
    if (_ie_core_debug_memory_dynamicarray == NULL)
        return realloc(p_old, size);

    void* p_new = realloc(p_old, size);
    
    int i;
    for (i = 0; i < iec_array_dynamic_size(_ie_core_debug_memory_dynamicarray); i++) {
        ie_debug_memory_allocation_record* record = iec_array_dynamic_value_pointer(_ie_core_debug_memory_dynamicarray, i);
        if (record->pdata == p_old) {
            record->pdata = p_new;
            record->size = size;
            record->line = line;
            int i;
            for (i = 0; i < 255 && file[i] != 0; ++i)
               record->file[i] = file[i];
        }
    }
    
    return p_new;
}


void iec_debug_memory_free(void* pointer)
{
    if (_ie_core_debug_memory_dynamicarray == NULL) {
        free(pointer);
        return;
    }

    int i;
    for (i = 0; i < iec_array_dynamic_size(_ie_core_debug_memory_dynamicarray); i++) {
        ie_debug_memory_allocation_record* record = iec_array_dynamic_value_pointer(_ie_core_debug_memory_dynamicarray, i);
        if (record->pdata == pointer)
            iec_array_dynamic_erase(_ie_core_debug_memory_dynamicarray, i);
    }

    free(pointer); 
}


void iec_debug_memory_print()
{
    printf("\n|--> Debug Memory Log - Start\n");

    unsigned int size_total = 0;
    unsigned int allocation_count = 0;
    int i;
    for (i = 0; i < iec_array_dynamic_size(_ie_core_debug_memory_dynamicarray); i++) {
        ie_debug_memory_allocation_record* record = iec_array_dynamic_value_pointer(_ie_core_debug_memory_dynamicarray, i);
        size_total += record->size;
        allocation_count++;
        printf("| ~ Record (%p) - File: (%s) Line: (%u) Size (%u)\n", record->pdata, record->file, record->line, record->size); 
    }
    
    printf("| Allocations (%u) / Size[bytes] (%u)\n", allocation_count, size_total);
    printf("|--> Debug Memory Log - End\n\n");
}


void iec_debug_memory_clear()
{
    iec_array_dynamic_clear(_ie_core_debug_memory_dynamicarray);
}

