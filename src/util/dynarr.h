#ifndef DYNARR_H
#define DYNARR_H
#include <stddef.h>
#include <stdint.h>
#define DYNARR_COMB1(a, b) DYNARR_COMB2(a, b)
#define DYNARR_COMB2(a, b) a##b
#endif // DYNARR_H

#ifdef DYNARR_TYPE
#include <stdlib.h>

// customising the linkage
#ifndef DYNARR_LINKAGE
#define DYNARR_LINKAGE static inline
#endif

// customising the name
#ifndef DYNARR_NAME
#define DYNARR_NAME DYNARR_COMB1(dynarr_, DYNARR_TYPE)
#endif // DYNARR_NAME

#define DYNARR_init   DYNARR_COMB1(DYNARR_NAME, _init)   // name of the initialization funcition
#define DYNARR_add    DYNARR_COMB1(DYNARR_NAME, _add)    // name of the addition function
#define DYNARR_remove DYNARR_COMB1(DYNARR_NAME, _remove) // name of the removal function
#define DYNARR_free   DYNARR_COMB1(DYNARR_NAME, _free)   // name of the freeing function

// define the dynamic array structure
typedef struct {
    DYNARR_TYPE* dat; // contains the data of the dynamic array
    size_t count;     // contains the amount of elements of the dynamic array
    size_t cap;       // contains the capacity of the dynamic array
} DYNARR_NAME;

// initializes the dynamic array with a specific capacity (dat may be null if malloc fails)
DYNARR_LINKAGE DYNARR_NAME DYNARR_init(const size_t cap) {
    return (DYNARR_NAME){
        malloc(cap * sizeof(DYNARR_TYPE)),
        0,
        cap,
    };
}

// adds an item to the dynamic array, doubles the capacity if the new count exceeds the maximum
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_add(DYNARR_NAME* arr, DYNARR_TYPE item) {
    size_t idx = arr->count;
    arr->count++;

    // resize the dynamic array if the new count has hit the max capacity
    if (arr->cap == arr->count) {
        arr->cap *= 2;
        DYNARR_TYPE* nptr = realloc(arr->dat, arr->cap * sizeof(DYNARR_TYPE));

        if (nptr == NULL)
            return 1;

        arr->dat = nptr;
    }

    arr->dat[idx] = item;
    return 0;
}

// removes an item from the dynamic array from a certain index
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_remove(DYNARR_NAME* arr, size_t idx) {
    if (arr->count == 0 || idx >= arr->count) return 1;

    // remove one from the count
    arr->count--;

    // update the subsequent items until the count (don't bother with the rest)
    // will only run if there are subsequent items
    for (size_t i = idx; i < arr->count; i++) {
        arr->dat[i] = arr->dat[i + 1]; // +1 is fine, we know the last item exists because we removed 1
    }

    return 0;
}

// cleans up the resources associated with the array, do not use after this step. This is undefined behaviour
DYNARR_LINKAGE void DYNARR_free(DYNARR_NAME* arr) {
    free(arr->dat);
    arr->dat = NULL;
}

// clean up all defined definitions so they can be used again later
#undef DYNARR_NAME
#undef DYNARR_init
#undef DYNARR_add
#undef DYNARR_remove
#undef DYNARR_PREFIX
#undef DYNARR_LINKAGE
#undef DYNARR_TYPE
#else
#if __INCLUDE_LEVEL__ != 0
#error define DYNARR_TYPE before including
#endif // include level
#endif // DYNARR_TYPE
