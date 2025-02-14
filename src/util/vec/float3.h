#pragma once

#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
} float3;

// adds two float3s together
static inline float3 float3_add(float3 v1, float3 v2) {
    return (float3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// subtracts one float3 from another
static inline float3 float3_sub(float3 v1, float3 v2) {
    return (float3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// multiplies the float3 by some value
static inline float3 float3_mlt(float3 v, float n) {
    return (float3){v.x * n, v.y * n, v.z * n};
}

// divides the float3 by some value
static inline float3 float3_div(float3 v, float n) {
    return (float3){v.x / n, v.y / n, v.z / n};
}

// gets the squared length of float3
static inline float float3_len2(float3 v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

// gets the length of float3 (magnitude)
static inline float float3_len(float3 v) {
    return sqrtf(float3_len2(v));
}

// normalizes the float3
static inline float3 float3_norm(float3 v) {
    float len = float3_len(v);
    return float3_div(v, len);
}

// gets the dot product of two float3s
static inline float float3_dot(float3 v1, float3 v2) {
    return v1.x * v2.x + v1.y * v2.y +v1.z * v2.z;
}
