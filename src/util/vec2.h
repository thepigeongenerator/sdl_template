#ifndef VEC2_H
#define VEC2_H
#define VEC2_COMB2(a, b) a##b
#define VEC2_COMB1(a, b) VEC2_COMB2(a, b)
#endif

#define VEC2_TYPE int

#ifdef VEC2_TYPE

// customising the linkage
#ifndef VEC2_LINKAGE
#define VEC2_LINKAGE static inline
#endif

// customising the name
#ifndef VEC2_NAME
#define VEC2_NAME VEC2_COMB1(vec2_, VEC2_TYPE)
#endif // VEC2_NAME

// defines a two-dimensional vector of a specific type
typedef struct {
    VEC2_TYPE x;
    VEC2_TYPE y;
} VEC2_NAME;

// TODO: implement vector-based logic

#undef VEC2_NAME
#undef VEC2_LINKAGE
#undef VEC2_TYPE
#elif __INCLUDE_LEVEL__ != 0
#error define VEC2_TYPE before including
#endif // VEC2_TYPE
