#ifndef DYNARR_H
#define DYNARR_H
#include <stddef.h>
#include <stdint.h>

#define DYNARR_COMB2(a, b) a##b
#define DYNARR_COMB1(a, b) DYNARR_COMB2(a, b)
#endif // DYNARR_H

#ifdef DYNARR_TYPE
#include <stdlib.h>
#include <string.h>

// customising the linkage
#ifndef DYNARR_LINKAGE
#define DYNARR_LINKAGE static inline
#endif

// customising the name
#ifndef DYNARR_NAME
#define DYNARR_NAME DYNARR_COMB1(dynarr_, DYNARR_TYPE)
#endif // DYNARR_NAME

#define DYNARR_COUNT_MAX  (SIZE_MAX / sizeof(DYNARR_TYPE))
#define DYNARR_FUNC(name) DYNARR_COMB1(DYNARR_NAME, _##name) // name of the initialization funcition

// define the dynamic array structure
typedef struct {
    DYNARR_TYPE* dat; // contains the data of the dynamic array
    size_t count;     // contains the amount of elements of the dynamic array
    size_t cap;       // contains the capacity of the dynamic array
} DYNARR_NAME;

// cleans up the resources associated with the array, do not use after this step. This is undefined behaviour
DYNARR_LINKAGE void DYNARR_FUNC(free)(DYNARR_NAME* arr) {
    free(arr->dat);          // free(NULL) is allowed
    *arr = (DYNARR_NAME){0}; // zero out all fields to re-initialize
}

// sets the capacity exactly, does not respect capacity scaling use `resize` if capacity scaling must be respected
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(resize_exact)(DYNARR_NAME* arr, size_t ncap) {
    if (ncap < arr->count) return 1; // the new capacity is smaller than the count, this is very likely unintentional
    if (ncap == arr->cap) return 0;  // the capacity is already the new capacity; no work needs to be done
    if (ncap == 0) {
        DYNARR_FUNC(free)(arr);
        return 0;
    }

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
    if (ncap < arr->count) return 1; // the new count is less than the current count, this is very likely unintentional
    if (ncap == arr->cap) return 0;  // the current capacity has already been set to this
    if (ncap == 0) {
        DYNARR_FUNC(free)(arr);
        return 0;
    }

    // convert the capacity into a power of 2 by selecting the most significan bit
    ncap--;                                       // first remove 1, to decrease the most significant bit
    for (uint16_t i = 1; i < SIZE_WIDTH; i <<= 1) // loop through each bit in size_t
        ncap |= ncap >> i;                        // OR together the shifted result
    ncap++;                                       // finally, add one so 0111 -> 1000

    // calculates what the new size should be by adding the amount of items to the count
    // assumes scaling factor is 2
    return DYNARR_FUNC(resize_exact)(arr, ncap);
}

// adds an item to the dynamic array, doubles the capacity if the new count exceeds the maximum
DYNARR_LINKAGE uint8_t DYNARR_FUNC(add_bulk)(DYNARR_NAME* arr, DYNARR_TYPE* dat, size_t datcount, size_t idx) {
    if (idx > arr->count) return 1;                         // the index is greater than the count
    if (DYNARR_COUNT_MAX - datcount < arr->count) return 1; // the count will overflow
    if (datcount == 0) return 1;                            // the count is zero

    size_t orgcount = arr->count;
    arr->count += datcount;

    // resize the array if the new count has hit the capacity
    if (arr->cap <= arr->count) {
        if (DYNARR_COUNT_MAX - arr->cap < arr->cap) return 1; // capacity will overflow

        // resize the capacity, store status in s
        uint8_t const s = !arr->cap
                              ? DYNARR_FUNC(resize_exact)(arr, 1)             // set the capacity to 1 if it currently is 0
                              : DYNARR_FUNC(resize_exact)(arr, arr->cap * 2); // otherwise, multiply the capacity by 2
        if (s) return 1;
    }

    // move the data stored at the current position if we must insert
    if (idx < orgcount)
        memmove(&arr->dat[datcount + idx], &arr->dat[idx], (orgcount - idx) * sizeof(DYNARR_TYPE));
    memcpy(&arr->dat[idx], dat, datcount * sizeof(DYNARR_TYPE)); // copy the original data to the index
    return 0;
}

// adds an item to the dynamic array, doubles the capacity if the new count exceeds the maximum
// returns 0 upon success, 1 upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(add)(DYNARR_NAME* arr, DYNARR_TYPE item) {
    return DYNARR_FUNC(add_bulk)(arr, &item, 1, arr->count);
}

// trims the parts of the dynamic array that isn't in use (does not respect scaling)
DYNARR_LINKAGE uint8_t DYNARR_FUNC(shrink)(DYNARR_NAME* arr) {
    if (arr->cap == arr->count) return 0; // return success if no work needs to be done
    return DYNARR_FUNC(resize_exact)(arr, arr->count);
}

// removes a block of indices from sidx..eidx (inclusive)
// resizes the array if the new size is a quarter of the original size
// returns non-zero value upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(remove_bulk)(DYNARR_NAME* arr, size_t sidx, size_t eidx) {
    if (arr->count == 0 || sidx >= arr->count || eidx < sidx) return 1;
    size_t diff = eidx - sidx + 1; // should always be less than or equal to count
    arr->count -= diff;

    for (size_t i = sidx; i < arr->count; i++) {
        arr->dat[i] = arr->dat[i + diff]; // this should be fine as we removed this amount
    }

    // shrink the array when the new size is a quarter of the original size
    if (arr->count < arr->cap / 4)
        return DYNARR_FUNC(shrink)(arr);

    return 0;
}

// removes an item from the dynamic array from a certain index
// resizes the array if the new size is a quarter of the original size
// returns non-zero value upon failure
DYNARR_LINKAGE uint8_t DYNARR_FUNC(remove)(DYNARR_NAME* arr, size_t idx) {
    return DYNARR_FUNC(remove_bulk)(arr, idx, idx);
}

// clean up all defined definitions so they can be used again later
#undef DYNARR_COUNT_MAX
#undef DYNARR_FUNC
#undef DYNARR_NAME
#undef DYNARR_LINKAGE
#undef DYNARR_TYPE
#elif __INCLUDE_LEVEL__ != 0
#error define DYNARR_TYPE before including
#endif // DYNARR_TYPE
