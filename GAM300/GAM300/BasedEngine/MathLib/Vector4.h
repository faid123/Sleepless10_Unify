#pragma once

#ifndef VECTOR4_H_
#define VECTOR4_H_



#include "Vector3.h"


#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

struct Vector4;
typedef const Vector4& Vec4Param;
typedef Vector4& Vec4Ref;
typedef Vector4* Vec4Ptr;

struct Vector4
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

    static const Vector4 cZero;
    static const Vector4 cXAxis;
    static const Vector4 cYAxis;
    static const Vector4 cZAxis;
    static const Vector4 cWAxis;

    Vector4();
    Vector4(float);
    Vector4(float x, float y, float z, float w);
    Vector4(Vec2Param vec2, float z = 0.0f, float w = 0.0f);
    Vector4(Vec3Param vec3, float w = 0.0f);

    Vector4(const glm::vec4& rhs);
    Vector4(const physx::PxVec4& rhs);

    // Convert Vector4 to GlmVec4
    glm::vec4 Vec4toGlmVec4();
    physx::PxVec4 Vec4toPVex4();


    //Binary Assignment Operators (reals)
    void operator*=(float rhs);
    void operator/=(float rhs);

    //Binary Operators (Reals)

    //Binary Assignment Operators (vectors)
    void operator+=(Vec4Param rhs);
    void operator-=(Vec4Param rhs);
    void operator*=(Vec4Param rhs);
    void operator/=(Vec4Param rhs);

    //Binary Operators (vectors)

    //Binary Vector Comparisons

    //Vector component wise multiply and divide

    ///Set all of the values of this vector at once.
    float& operator[](unsigned index);

    void Set(float x, float y, float z, float w);

    ///Set all of the values of the vector to the passed in value.
    void Splat(float xyzw);

    ///Do a component-wise scaling of this vector with the given vector.
    void ScaleByVector(Vec4Param rhs);

    Vector4 ScaledByVector(Vec4Param rhs) const;

    void ZeroOut();
    void AddScaledVector(Vec4Param vector, float scalar);

    float Dot(Vec4Param rhs) const;
    float Length() const;
    float LengthSq() const;
    Vector4 Normalized() const;
    float Normalize();
    Vec4Ref Negate();

    //// Returns a vector that is made from the smallest components of two vectors.
    static Vector4 Min(const Vector4& lhs, const Vector4& rhs);
    //// Returns a vector that is made from the largest components of two vectors.
    static Vector4 Max(const Vector4& lhs, const Vector4& rhs);

    static void RegisterLua(sol::state& lua);
};

Vector4 operator*(float lhs, Vec4Param rhs);
float Dot(Vec4Param lhs, Vec4Param rhs);
float Length(Vec4Param vect);
float LengthSq(Vec4Param vect);
Vector4 Normalized(Vec4Param vect);
float Normalize(Vec4Ptr vect);
void Negate(Vec4Ptr vec);
Vector4 Negated(Vec4Param vec);
Vector4 Abs(Vec4Param vec);
//Vector4 Min(Vec4Param lhs, Vec4Param rhs);
//Vector4 Max(Vec4Param lhs, Vec4Param rhs);

//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

// Adds two vectors.
Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
// Subtracts one vector from another.
Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
// Negates a vector.
Vector4 operator-(const Vector4& lhs);
// Is equals to operator
bool operator==(const Vector4& lhs, const Vector4& rhs);
// Not equals to operator
bool operator!=(const Vector4& lhs, const Vector4& rhs);
// Multiplies a vector by a number.
Vector4 operator*(const Vector4& lhs, float rhs);
Vector4 operator*(const Vector4& lhs, const Vector4& rhs);
// Divides a vector by a number.
Vector4 operator/(const Vector4& lhs, float rhs);
Vector4 operator/(const Vector4& lhs, const Vector4& rhs);


#endif