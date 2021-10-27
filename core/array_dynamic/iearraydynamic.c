#include "iearraydynamic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _iec_array_dynamic_grow(iec_array_dynamic* array)
{
    array->element_amount_capacity *= array->growth_factor;
    array->pdata = realloc(array->pdata, array->element_amount_capacity * array->element_size);
}


iec_array_dynamic* iec_array_dynamic_create(size_t element_size, unsigned int capacity, float growth_factor)
{
    iec_array_dynamic* new_array = malloc(sizeof(iec_array_dynamic));

    new_array->element_size = element_size;
    new_array->element_amount_used = 0;
    new_array->element_amount_capacity = capacity;
    new_array->growth_factor = growth_factor;

    new_array->pdata = malloc(new_array->element_amount_capacity * element_size);

    return new_array;
}


void iec_array_dynamic_destroy(iec_array_dynamic* array)
{
    free(array->pdata);
    free(array);
}


void* iec_array_dynamic_push_back(iec_array_dynamic* array, void* data)
{
    if (array->element_amount_used == array->element_amount_capacity)
        _iec_array_dynamic_grow(array);

    void* p_destination = array->pdata + (array->element_size * array->element_amount_used); 
    memcpy(p_destination, data, array->element_size); 
    array->element_amount_used++;
    
    return p_destination; 
}


void iec_array_dynamic_pop_back(iec_array_dynamic* array, void* data)
{
    if (array->element_amount_used == 0) {
        data = NULL;
        return;
    }
    
    array->element_amount_used--;

    if ( data != NULL ) 
        memcpy(data, array->pdata + (array->element_size * array->element_amount_used), array->element_size);
}


void iec_array_dynamic_value(iec_array_dynamic* array, void* data, unsigned int index)
{
    if (index >= array->element_amount_used && index >= 0) {
       data = NULL;
       return;
    }

    void* p_source = array->pdata + (array->element_size * index );
    memcpy(data, p_source, array->element_size);
}


void* iec_array_dynamic_value_pointer(iec_array_dynamic* array, unsigned int index)
{
    if (index >= array->element_amount_used || index < 0)
       return NULL;

    return array->pdata + (array->element_size * index );
}


void* iec_array_dynamic_assign(iec_array_dynamic* array, void* data, unsigned int index)
{
    if (index >= array->element_amount_used || index < 0)
       return NULL;

    void* p_destination = array->pdata + (array->element_size * index );
    memcpy(p_destination, data, array->element_size);

    return p_destination;
}


void* iec_array_dynamic_insert(iec_array_dynamic* array, void* data, unsigned int index)
{
    if (index >= array->element_amount_used || index < 0)
       return NULL;
    
    if (array->element_amount_used == array->element_amount_capacity)
        _iec_array_dynamic_grow(array);

    void* p_destination = array->pdata + (array->element_size * (index + 1) );
    void* p_end = array->pdata + (array->element_size * array->element_amount_used);
    void* p_source = array->pdata + (array->element_size * index);    
    size_t nbytes = p_end - p_source;

    memcpy(p_destination, p_source, nbytes);
    memcpy(p_source, data, array->element_size);
    array->element_amount_used++;

    return p_source;
}


void iec_array_dynamic_erase(iec_array_dynamic* array, unsigned int index)
{
   if (index >= array->element_amount_used || index < 0)
       return;
    
   void* p_destination = array->pdata + (array->element_size * index);
   void* p_end = array->pdata + (array->element_size * array->element_amount_used);
   void* p_source = p_destination + array->element_size;    

   array->element_amount_used--;

   if (p_source >= p_end)
       return;

   size_t nbytes = p_end - p_source;

   memcpy(p_destination, p_source, nbytes);
}


void iec_array_dynamic_clear(iec_array_dynamic* array)
{
    array->element_amount_used = 0;
}


void iec_array_dynamic_refit(iec_array_dynamic* array)
{
    array->pdata = realloc(array->pdata, array->element_amount_used * array->element_size);
    array->element_amount_capacity = array->element_amount_used;
}


void iec_array_dynamic_resize(iec_array_dynamic* array, unsigned int new_capacity)
{
    if (array->element_amount_capacity == new_capacity)
        return;

    if (array->element_amount_used >= new_capacity) {
        iec_array_dynamic_refit(array);
        return;
    }

    array->pdata = realloc(array->pdata, new_capacity * array->element_size);
    array->element_amount_capacity = new_capacity;
}


unsigned int iec_array_dynamic_is_empty(iec_array_dynamic* array)
{
    return array->element_amount_used;
}


unsigned int iec_array_dynamic_size(iec_array_dynamic* array)
{
    return array->element_amount_used;
}


unsigned int iec_array_dynamic_capacity(iec_array_dynamic* array)
{
    return array->element_amount_capacity;
}


