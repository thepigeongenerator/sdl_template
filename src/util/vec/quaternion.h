#include "../../error.h"
#include "float3.h"
#include "float4.h"


// converts euler angles into quaternion
static inline float4 quat_from_euler(float3 euler) {
    (void)euler;
    error(STATUS_ERROR, __FILE_NAME__, __LINE__, "this function is not implemented"); // TODO: write an implementation for this function
}

// converts quaternion into euler angles
static inline float3 quat_to_euler(float4 q) {
    (void)q;
    error(STATUS_ERROR, __FILE_NAME__, __LINE__, "this function is not implemented"); // TODO: write an implementation for this function
}

// adds two quaternions
static inline float4 quat_add(float4 q1, float4 q2) {
    (void)q1, (void)q2;
    error(STATUS_ERROR, __FILE_NAME__, __LINE__, "this function is not implemented"); // TODO: write an implementation for this function
}

// subtracts two quaternions
static inline float4 quat_sub(float4 q1, float4 q2) {
    (void)q1, (void)q2;
    error(STATUS_ERROR, __FILE_NAME__, __LINE__, "this function is not implemented"); // TODO: write an implementation for this function
}

// multiplies two quaternions
static inline float4 quat_mul(float4 q1, float4 q2) {
    return (float4){
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, // x
        q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, // y
        q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, // z
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z, // w
    };
}

// multiplies two quaternions
static inline float4 quat_div(float4 q1, float4 q2) {
    (void)q1, (void)q2;
    error(STATUS_ERROR, __FILE_NAME__, __LINE__, "this function is not implemented"); // TODO: write an implementation for this function
}
