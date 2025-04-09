#include <assert.h>
#include <math.h>

#include "../attributes.h"
#include "float3.h"
#include "float4.h"

// converts a float3 to a quaternion
static inline float4 quat_from_float3(float3 v) {
	return (float4){
		.w = 0.0F,
		.x = v.x,
		.y = v.y,
		.z = v.z,
	};
}

// converts euler angles into quaternion (ordered roll, pitch, yaw) (in radians)
atrb_const static inline float4 quat_from_euler(float3 euler) {
	euler = float3_mul_s(euler, 0.5F); // half the angles due to quaternions using θ/2 in the formula
	float cx = cosf(euler.x), sx = sinf(euler.x);
	float cy = cosf(euler.y), sy = sinf(euler.y);
	float cz = cosf(euler.z), sz = sinf(euler.z);

	return (float4){
		.w = cx * cy * cz - sx * sy * sz,
		.x = sx * cy * cz + cx * sy * sz,
		.y = cx * sy * cz - sx * cy * sz,
		.z = cx * cy * sz + sx * sy * cz,
	};
}

// converts quaternion into euler angles (ordered as roll, pitch, yaw)
atrb_const static inline float3 quat_to_euler(float4 q) {
	// warn: do not read from these variables until set
	float3 euler;
	float a, b;

	// compute the roll (Φ)
	a = 2 * (q.w * q.x + q.y * q.z);     // sin(r)•cos(p)
	b = 1 - 2 * (q.x * q.x + q.y * q.y); // cos(r)•cos(p)
	euler.x = atan2f(a, b);

	// compute the pitch (θ)
	a = 2 * (q.w * q.y - q.z * q.x);
	euler.y = fabsf(a) >= 1 ? copysignf(M_PI_2, a) : asinf(a); // if |a| >=1, sgn(a)•(π/2), else asin(a)

	// compute the yaw (ψ)
	a = 2 * (q.w * q.z + q.x * q.y);     // sin(y)•cos(y)
	b = 1 - 2 * (q.y * q.y + q.z * q.z); // cos(y)•cos(y)
	euler.z = atan2f(a, b);

	// verify that all axis are less than or equal to τ (tau)
	assert(euler.x <= M_2_PIf);
	assert(euler.y <= M_2_PIf);
	assert(euler.z <= M_2_PIf);

	// return the final angles
	return euler;
}

// multiplies two quaternions
atrb_const static inline float4 quat_mul(float4 q1, float4 q2) {
	return (float4){
		.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
	};
}

// get the conjugate of the quaternion (negates the vector portion)
atrb_const static inline float4 quat_conj(float4 q) {
	return (float4){
		.w = q.w,
		.x = -q.x,
		.y = -q.y,
		.z = -q.z,
	};
}

// get the multiplicative inverse of the quaternion (conj / mag²)
atrb_const static inline float4 quat_inv(float4 q) {
	float mag2 = float4_mag2(q);
	if (mag2 == 0.0F) return (float4){0};
	mag2 = 1.0F / mag2;
	return float4_mul_s(quat_conj(q), mag2);
}

// rotates a vector by the quaternion (q must be a unit quaternion (normalized))
atrb_const static inline float3 quat_rot(float4 q, float3 v) {
	assert(fabsf(1.0F - (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w)) < 0.005F); // assert whether the quaternion is a unit quaternion (within a margin of error due to float precision)
	q = quat_mul(quat_mul(q, quat_from_float3(v)), quat_conj(q));                   // q•v•q¯¹ (using conjugate for q⁻¹, as for unit quaternions this is the same as the multiplicative inverse)
	return (float3){q.x, q.y, q.z};
}

// rotates a vector by the quaternion, q may be non-normalized
atrb_const static inline float3 quat_rot_s(float4 q, float3 v) {
	q = quat_mul(quat_mul(q, quat_from_float3(v)), quat_inv(q)); // q•v•q¯¹
	return (float3){q.x, q.y, q.z};
}
