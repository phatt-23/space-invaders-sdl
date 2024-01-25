#pragma once

typedef void* array_type;

typedef struct dynarray DynArray;
struct dynarray {
    array_type* items;      // Adresa s pamětí pro prvky pole
    int size;               // Kolik prvků je aktuálně v poli
    int capacity;           // Kolik prvků se aktuálně vejde to pole
};

/**
 * Initialises a dynamic array and allocates an initial capacity
 * in bytes.
 *  - example call:
 *      dynarray( &ArrayOfPointersToSDL_Textures, sizeof(SDLTexture*) );
 *      dynarray( &GameObjects, sizeof(GameObject) );
 *
 **/
void dynarray_init(DynArray* array, int initial_capacity);

/**
 * Add an element to an array.
 * The will be added element must be allocated via malloc.
 *      (The thing you want to put in must be manually allocated
 *       on the heap, e.i. not be normally created on the stack.)
 *  e.g.:
 *      GameObject* new_game_object = malloc( sizeof(GameObject) );
 *      // code to initialise and set up the new_game_object //
 *      dynarray_push( &GameObjects, new_game_object );
 *
 **/
void dynarray_push(DynArray* array, array_type item);

/**
 * Remove a certain element from the array.
 * This will simultaneously free the memory of the removed element.
 **/
void dynarray_remove(DynArray* array, array_type item);

/**
 * Free the dynamic array from memory.
 **/
void dynarray_free(DynArray* array);
