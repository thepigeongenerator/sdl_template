#pragma once

#include <math.h>

// stores a 3D point using a floating-point number
typedef struct {
    float x;
    float y;
    float z;
    float w;
} float4;

// adds one float4 to another
static inline float4 float4_add(float4 v1, float4 v2) {
    return (float4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// subtracts one float4 from another
static inline float4 float4_sub(float4 v1, float4 v2) {
    return (float4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// multiplies one float4 by another
static inline float4 float4_mlt(float4 v1, float4 v2) {
    return (float4){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

// scales the float4 by some value
static inline float4 float4_scale(float4 v, float n) {
    return (float4){v.x * n, v.y * n, v.z * n, v.w * n};
}

// divides the float4 by some value
static inline float4 float4_div(float4 v, float n) {
    return (float4){v.x / n, v.y / n, v.z / n, v.w / n};
}

// gets the squared length of float4
static inline float float4_len2(float4 v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

// gets the length of float4 (magnitude)
static inline float float4_len(float4 v) {
    return sqrtf(float4_len2(v));
}

// normalizes the float4
static inline float4 float3_norm(float4 v) {
    float len = float4_len(v);
    return float4_div(v, len);
}

// gets the dot product of two float4s
static inline float float4_dot(float4 v1, float4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
