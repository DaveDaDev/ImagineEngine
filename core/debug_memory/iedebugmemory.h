#ifndef IE_CORE_DEBUG_MEMORY_H
#define IE_CORE_DEBUG_MEMORY_H

#include <stddef.h>

typedef struct {
    unsigned int line;
    unsigned int size;
    char file[256];
    void* pdata;
} ie_debug_memory_allocation_record;

void  iec_debug_memory_startup();
void  iec_debug_memory_shutdown();

void* iec_debug_memory_malloc(size_t size, char* file, unsigned int line);
void* iec_debug_memory_realloc(void* pointer, size_t size, char* file, unsigned int line);
void  iec_debug_memory_free(void* pointer);

#ifdef IE_CORE_DEBUG_MEMORY_STATUS_ON

#define malloc(s) iec_debug_memory_malloc(s, __FILE__, __LINE__)
#define realloc(p, s) iec_debug_memory_realloc(p, s, __FILE__, __LINE__)
#define free(p) iec_debug_memory_free(p)

#endif

void  iec_debug_memory_print();
void  iec_debug_memory_clear();

#endif
