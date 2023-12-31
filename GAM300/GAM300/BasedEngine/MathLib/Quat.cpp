#include "Precompiled.h"
#include <glm/gtc/quaternion.hpp>
#include "PxPhysics.h"
#include <corecrt_math_defines.h>

static constexpr float DegToRad = 3.14159265358979323846264338327950288f * 2.0f / 360.0f;
static constexpr float RadToDeg = 1.0f / DegToRad;

Quat::Quat() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
{
}

Quat::Quat(float _x, float _y, float _z, float _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w }
{
}

Quat::Quat(Quat& rhs) 
{
    x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
}

Quat::Quat(const glm::quat& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}

Quat::Quat(const physx::PxQuat& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}

glm::quat Quat::QuattoGlmQuat()
{
    return glm::quat(w, x, y, z);
}

physx::PxQuat Quat::QuattoPQuat()
{
    return physx::PxQuat(x, y, z, w);
}

Quat& Quat::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

float Quat::Length() const
{
	return std::sqrt(LengthSq());
}

float Quat::LengthSq() const
{
    return x * x + y * y + z * z + w * w;
}

float Quat::Dot(Quat a, Quat b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w + b.w;
}

float Quat::Angle(Quat a, Quat b)
{
	float f = Quat::Dot(a, b);
	return std::acosf(std::min(std::abs(f), 1.0f)) * 2.0f * RadToDeg;
}

Quat Quat::Normalize(Quat input)
{
	float scale = 1.0f / input.Length();
	Quat result = input;
	result.x *= scale;
	result.y *= scale;
	result.z *= scale;
	result.w *= scale;
	return result;
}

Quat Quat::Slerp(Quat& a, Quat& b, float t)
{
	if (t > 1) 
		t = 1;

	if (t < 0) 
		t = 0;

	return Quat::SlerpUnclamped(a, b, t);
}

Quat Quat::SlerpUnclamped(Quat& a, Quat& b, float t)
{
	// if either input is zero, return the other.
	if (a.LengthSq() == 0.0f)
	{
		if (b.LengthSq() == 0.0f)
		{
			return Quat{ 0.0, 0.0, 0.0, 1.0 };
		}
		return b;
	}
	else if (b.LengthSq() == 0.0f)
	{
		return a;
	}

	Vector3 aVec = { a.x, a.y, a.z };
	Vector3 bVec = { b.x, b.y, b.z };

	float cosHalfAngle = a.w * b.w + Vector3::Dot(aVec, bVec);

	if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
	{
		// angle = 0.0f, so just return one input.
		return a;
	}
	else if (cosHalfAngle < 0.0f)
	{
		b.x = -b.x;
		b.y = -b.y;
		b.z = -b.z;
		b.w = -b.w;
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	if (cosHalfAngle < 0.99f)
	{
		// do proper slerp for big angles
		float halfAngle = std::acosf(cosHalfAngle);
		float sinHalfAngle = std::sinf(halfAngle);
		float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
		blendA = std::sinf(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
		blendB = std::sinf(halfAngle * t) * oneOverSinHalfAngle;
	}
	else
	{
		// do lerp if angle is really small.
		blendA = 1.0f - t;
		blendB = t;
	}

	Quat result = { blendA * a.x + blendB * b.x, blendA * a.y + blendB * b.y, blendA * a.z + blendB * b.z, blendA * a.w + blendB * b.w };
	if (result.LengthSq() > 0.0f)
		return Normalize(result);
	else
		return Quat{ 0.0, 0.0, 0.0, 1.0 };
}

Quat Quat::RotateTowards(Quat origin, Quat target, float t)
{
	float num = Quat::Angle(origin, target);
	if (num == 0.0f)
	{
		return target;
	}
	float _t = std::min(1.0f, t / num);
	return Quat::SlerpUnclamped(origin, target, _t);
}

//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

void Normalize(Quat& lhs)
{
    lhs /= sqrt(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w);
}

Quat Euler_to_Quat(Vector3& euler)
{
    float roll = euler.x * ((float)M_PI / 180.0f);
    float pitch = euler.y * ((float)M_PI / 180.0f);
    float yaw = euler.z * ((float)M_PI / 180.0f);

    // Abbreviations for the various angular functions
    float cy = cosf(yaw * 0.5f);
    float sy = sinf(yaw * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sp = sinf(pitch * 0.5f);
    float cr = cosf(roll * 0.5f);
    float sr = sinf(roll * 0.5f);

    float x = sr * cp * cy - cr * sp * sy;
    float y = cr * sp * cy + sr * cp * sy;
    float z = cr * cp * sy - sr * sp * cy;
    float w = cr * cp * cy + sr * sp * sy;

    return Quat(x, y, z, w);
}

//Quat Matrix4_to_Quat(Matrix4 m)
//{
//    float t;
//    Quat q;
//
//    if (m[2][2] < 0)
//    {
//        if (m[0][0] > m[1][1])
//        {
//            t = 1 + m[0][0] - m[1][1] - m[2][2];
//            q = quat(t, m[0][1] + m[1][0], m[2][0] + m[0][2], m[1][2] - m[2][1]);
//        }
//        else
//        {
//            t = 1 - m[0][0] + m[1][1] - m[2][2];
//            q = quat(m[0][1] + m[1][0], t, m[1][2] + m[2][1], m[2][0] - m[0][2]);
//        }
//    }
//    else
//    {
//        if (m[0][0] < -m[1][1])
//        {
//            t = 1 - m[0][0] - m[1][1] + m[2][2];
//            q = quat(m[2][0] + m[0][2], m[1][2] + m[2][1], t, m[0][1] - m[1][0]);
//        }
//        else
//        {
//            t = 1 + m[0][0] + m[1][1] + m[2][2];
//            q = quat(m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0], t);
//        }
//    }
//
//    q.x *= 0.5 / sqrtf(t);
//    q.y *= 0.5 / sqrtf(t);
//    q.z *= 0.5 / sqrtf(t);
//    q.w *= 0.5 / sqrtf(t);
//
//    return q;
//}