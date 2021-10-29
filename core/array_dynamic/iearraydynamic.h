#ifndef IE_CORE_DYNAMIC_ARRAY_H
#define IE_CORE_DYNAMIC_ARRAY_H

#include <stdlib.h>

#define IEC_ARRAY_DYNAMIC_DEFAULT_GROWTH_RATIO 1.62
#define IEC_ARRAY_DYNAMIC_DEFAULT_ELEMENT_CAPACITY 8

typedef struct {
    void*        pdata;
    unsigned int element_amount_capacity;
    unsigned int element_amount_used;
    unsigned int dynamic_shrink;
    float        growth_factor;
    size_t       element_size;
} iec_array_dynamic;

iec_array_dynamic* iec_array_dynamic_create(size_t element_size);
void               iec_array_dynamic_destroy(iec_array_dynamic* array);

void*              iec_array_dynamic_push_back(iec_array_dynamic* array, void* pdata);
void               iec_array_dynamic_pop_back(iec_array_dynamic* array, void* pdata);
void               iec_array_dynamic_value(iec_array_dynamic* array, void* pdata, unsigned int index);
void*              iec_array_dynamic_value_pointer(iec_array_dynamic* array, unsigned int index);
void*              iec_array_dynamic_assign(iec_array_dynamic* array, void* pdata, unsigned int index);
void*              iec_array_dynamic_insert(iec_array_dynamic* array, void* pdata, unsigned int index);
void               iec_array_dynamic_erase(iec_array_dynamic* array, unsigned int index);              
void               iec_array_dynamic_clear(iec_array_dynamic* array);

void               iec_array_dynamic_refit(iec_array_dynamic* array);
void               iec_array_dynamic_resize(iec_array_dynamic* array, unsigned int capacity);
void               iec_array_dynamic_growth_set(iec_array_dynamic* array, float new_rate);
void               iec_array_dynamic_shrink_toggle(iec_array_dynamic* array, unsigned int toggle);

unsigned int       iec_array_dynamic_is_empty(iec_array_dynamic* array);
unsigned int       iec_array_dynamic_size(iec_array_dynamic* array);
unsigned int       iec_array_dynamic_capacity(iec_array_dynamic* array);
unsigned int       iec_array_dynamic_has(iec_array_dynamic* array, void* pdata,  unsigned int (*ie_equal)(void*, void*));

#endif
