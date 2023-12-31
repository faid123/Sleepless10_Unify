#pragma once

#ifndef QUAT_H_
#define QUAT_H_

#include "Vector3.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

struct Vector3;
struct Quat;

struct Quat
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float array[4];
	};

	Quat();
	Quat(float _x, float _y, float _z, float _w);
	Quat(Quat& rhs);
	Quat(const Quat& rhs) :
		x{rhs.x},
		y{rhs.y},
		z{rhs.z},
		w{rhs.w}
	{}
	Quat(const glm::quat& rhs);
	Quat(const physx::PxQuat& rhs);

	glm::quat QuattoGlmQuat();
	physx::PxQuat QuattoPQuat();

    static void RegisterLua(sol::state& lua);

	Quat& operator/=(float rhs);


	float Length() const;
	float LengthSq() const;

	static float Dot(Quat a, Quat b);

	static float Angle(Quat a, Quat b);
	
	static Quat Normalize(Quat input);

	static Quat Slerp(Quat& a, Quat& b, float t);
	static Quat SlerpUnclamped(Quat& a, Quat& b, float t);

	static Quat RotateTowards(Quat origin, Quat target, float t);
};



//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

// Adds two vectors.
void Normalize(Quat& lhs);

Quat Euler_to_Quat(Vector3& euler);

//Quat Matrix4_to_Quat(Matrix4 mtx);

#endif