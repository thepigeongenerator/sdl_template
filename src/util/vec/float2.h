#pragma once

#include <math.h>

typedef struct {
    float x;
    float y;
} float2;

// adds two float2s together
static inline float2 float2_add(float2 v1, float2 v2) {
    return (float2){v1.x + v2.x, v1.y + v2.y};
}

// subtracts one float2 from another
static inline float2 float2_sub(float2 v1, float2 v2) {
    return (float2){v1.x - v2.x, v1.y - v2.y};
}

// multiplies the float2 by some value
static inline float2 float2_mlt(float2 v, float n) {
    return (float2){v.x * n, v.y * n};
}

// divides the float2 by some value
static inline float2 float2_div(float2 v, float n) {
    return (float2){v.x / n, v.y / n};
}

// gets the squared length of float2
static inline float float2_len2(float2 v) {
    return (v.x * v.x) + (v.y * v.y);
}

// gets the length of float2 (magnitude)
static inline float float2_len(float2 v) {
    return sqrtf(float2_len2(v));
}

// normalizes the float2
static inline float2 float2_norm(float2 v) {
    float len = float2_len(v);
    return float2_div(v, len);
}

// gets the dot product of two float2s
static inline float float2_dot(float2 v1, float2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
