#pragma once

#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
    float w;
} float4;

// adds two float3s together
static inline float4 float3_add(float4 v1, float4 v2) {
    return (float4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// subtracts one float3 from another
static inline float4 float3_sub(float4 v1, float4 v2) {
    return (float4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// multiplies the float3 by some value
static inline float4 float3_mlt(float4 v, float n) {
    return (float4){v.x * n, v.y * n, v.z * n, v.w * n};
}

// divides the float3 by some value
static inline float4 float3_div(float4 v, float n) {
    return (float4){v.x / n, v.y / n, v.z / n, v.w / n};
}

// gets the squared length of float3
static inline float float3_len2(float4 v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

// gets the length of float3 (magnitude)
static inline float float3_len(float4 v) {
    return sqrtf(float3_len2(v));
}

// normalizes the float3
static inline float4 float3_norm(float4 v) {
    float len = float3_len(v);
    return float3_div(v, len);
}

// gets the dot product of two float3s
static inline float float3_dot(float4 v1, float4 v2) {
    return v1.x * v2.x + v1.y * v2.y +v1.z * v2.z + v1.w * v2.w;
}
