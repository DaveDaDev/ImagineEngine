#include "iearraydynamic.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    iec_array_dynamic* my_new_array = iec_array_dynamic_create(sizeof(int), IEC_ARRAY_DYNAMIC_DEFAULT_ELEMENT_AMOUNT, IEC_ARRAY_DYNAMIC_DEFAULT_GROWTH_RATIO);
    
    int i;
    for (i = 1; i < 21; ++i) { 
        iec_array_dynamic_push_back(my_new_array, &i);
    }

    int insert = 55;
    iec_array_dynamic_erase(my_new_array, 15);
    iec_array_dynamic_insert(my_new_array, &insert, 15);
    int assign = 69;
    iec_array_dynamic_assign(my_new_array, &assign, 20); 

    int* pdata = NULL; 
    for(i = 0; i < iec_array_dynamic_size(my_new_array); ++i) {
        pdata = iec_array_dynamic_value_pointer(my_new_array, i);
        if ( pdata != NULL )
            printf("%i\n", *pdata);
    }

    int pop = 0;

    iec_array_dynamic_pop_back(my_new_array, &pop);
    printf("Popped: %i\n", pop);
    iec_array_dynamic_pop_back(my_new_array, &pop);
    printf("Popped: %i\n", pop);

    printf("Capacity: %u\n", iec_array_dynamic_capacity(my_new_array));
    iec_array_dynamic_refit(my_new_array);
    printf("Refit\n");
    printf("Capacity: %u\n", iec_array_dynamic_capacity(my_new_array));

    for(i = 0; i < iec_array_dynamic_size(my_new_array); ++i) {
        pdata = iec_array_dynamic_value_pointer(my_new_array, i);
        if ( pdata != NULL )
            printf("%i\n", *pdata);
    }

    iec_array_dynamic_destroy(my_new_array);

    printf("Completed\n");
}
