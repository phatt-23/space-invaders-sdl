#include "dynamic_array.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void dynarray_init(DynArray* array, int initial_capacity) {
    array->items = (array_type*) malloc(sizeof(array_type) * initial_capacity);
    array->size = 0;
    array->capacity = initial_capacity;
    memset(array->items, 0, sizeof(array_type) * initial_capacity);
}

void dynarray_push(DynArray* array, array_type item) {
    if (array->capacity == array->size) {
        int new_capacity = array->capacity * 2;
        array_type* memory = (array_type*) malloc(sizeof(array_type) * new_capacity); // Vytvoření dvojnásobně velké paměti
        memcpy(memory, array->items, array->size * sizeof(array_type)); // Nakopírování staré paměti do nové paměti
        free(array->items); // Uvolnění staré paměti

        array->items = memory;
        array->capacity = new_capacity;
    }
    array->items[array->size++] = item;
}


void dynarray_remove(DynArray * array, array_type item) {
    for (int i = 0; i < array->size; i++) {
        if (array->items[i] == item) {
            // If the item is found, move all elements to the left by one position
            for (int j = i + 1; j < array->size; j++) {
                array->items[j - 1] = array->items[j];
            }
            array->size -= 1;
            // Adjust the loop counter to recheck the current index in case the same value is moved to this position
            i -= 1;
        }
    }
}

void dynarray_free(DynArray* array) {
    for (int i = 0; i < array->size; i++) {
        free(array->items[i]);
    }
    free(array->items);
}
