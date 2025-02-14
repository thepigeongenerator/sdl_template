#ifndef DYNARR_H
#define DYNARR_H
#include <stddef.h>
#include <stdint.h>

#define DYNARR_COMB2(a, b) a##b
#define DYNARR_COMB1(a, b) DYNARR_COMB2(a, b)
#endif // DYNARR_H

#define DYNARR_TYPE float

#ifdef DYNARR_TYPE
#include <math.h>
#include <stdlib.h>

// customising the linkage
#ifndef DYNARR_LINKAGE
#define DYNARR_LINKAGE static inline
#endif

// customising the name
#ifndef DYNARR_NAME
#define DYNARR_NAME DYNARR_COMB1(dynarr_, DYNARR_TYPE)
#endif // DYNARR_NAME

#define DYNARR_FUNC(name) DYNARR_COMB1(DYNARR_NAME, _##name) // name of the initialization funcition
// TODO: add shrink function (call when count < cap / 4)
// TODO: add bulk addition function
// TODO: add bulk deletion function


// define the dynamic array structure
typedef struct {
    DYNARR_TYPE* dat; // contains the data of the dynamic array
    size_t count;     // contains the amount of elements of the dynamic array
    size_t cap;       // contains the capacity of the dynamic array
} DYNARR_NAME;

// sets the capacity exactly, does not respect capacity scaling use `resize` if capacity scaling must be respected
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(resize_exact)(DYNARR_NAME* arr, size_t ncap) {
    if (ncap == 0) return 1;         // cannot set the new capacity to zero
    if (ncap < arr->count) return 1; // the new capacity is smaller than the count, this is very likely unintentional
    if (ncap == arr->cap) return 0;  // the capacity is already the new capacity; no work needs to be done

    // (re)allocate the memory for the array
    DYNARR_TYPE* nptr = realloc(arr->dat, ncap * sizeof(DYNARR_TYPE)); // if dat is NULL, behaviour is equivalent to "malloc"

    // if memory (re)allocation failed; return
    if (nptr == NULL)
        return 1;

    arr->dat = nptr;
    arr->cap = ncap;
    return 0;
}

// resizes the capacity, respects capacity scaling, use `resize_exact` if this behaviour isn't desirable (often it is)
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(resize)(DYNARR_NAME* arr, size_t ncap) {
    if (ncap == 0) return 1;         // cannot set the new capacity to zero
    if (ncap < arr->count) return 1; // the new count is less than the current count, this is very likely unintentional
    if (ncap == arr->cap) return 0;  // the current capacity has already been set to this

    // calculates what the new size should be by adding the amount of items to the count
    // assumes scaling factor is 2
    return DYNARR_FUNC(resize_exact)(arr, 1 << (size_t)ceil(log2(ncap)));
}

// adds an item to the dynamic array, doubles the capacity if the new count exceeds the maximum
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(add)(DYNARR_NAME* arr, DYNARR_TYPE item) {
    size_t idx = arr->count;
    arr->count++;

    // resize the dynamic array if the new count has hit the max capacity
    if (arr->cap <= arr->count) {
        // fail if the capacity will overflow
        if ((SIZE_MAX - arr->cap) < arr->cap) return 1;

        // resize the capacity, store the status in s
        uint8_t const s = !arr->cap
                              ? DYNARR_FUNC(resize_exact)(arr, 1)             // set the capacity to 1 if it's 0
                              : DYNARR_FUNC(resize_exact)(arr, arr->cap * 2); // otherwise, multiply the capacity by 2
        // return 1 upon non-zero
        if (s) return 1;
    }

    arr->dat[idx] = item;
    return 0;
}

// removes an item from the dynamic array from a certain index
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(remove)(DYNARR_NAME* arr, size_t idx) {
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

// trims the parts of the dynamic array that isn't in use
DYNARR_LINKAGE uint8_t DYNARR_FUNC(trim)(DYNARR_NAME* arr) {
    if (arr->cap == arr->count) return 0; // return success if no work needs to be done
    return DYNARR_FUNC(resize_exact)(arr, arr->count);
}

// cleans up the resources associated with the array, do not use after this step. This is undefined behaviour
DYNARR_LINKAGE void DYNARR_FUNC(free)(DYNARR_NAME* arr) {
    free(arr->dat);          // free(NULL) is allowed
    *arr = (DYNARR_NAME){0}; // zero out all fields to re-initialize
}

// clean up all defined definitions so they can be used again later
#undef DYNARR_FUNC
#undef DYNARR_NAME
#undef DYNARR_LINKAGE
#undef DYNARR_TYPE
#elif __INCLUDE_LEVEL__ != 0
#error define DYNARR_TYPE before including
#endif // DYNARR_TYPE
