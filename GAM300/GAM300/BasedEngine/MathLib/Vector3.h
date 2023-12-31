#pragma once

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "Vector2.h"

#include "Quat.h"

#include <glm/glm.hpp>
#include "PxPhysics.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include <functional>
#include <fmod.hpp>
#include <assimp/types.h>

struct Quat;
struct Vector3;
//using Vec3Param = const Vector3&;
typedef const Vector3& Vec3Param;
using Vec3Ref = Vector3&;
using Vec3Ptr = Vector3*;

struct Vector3
{
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    union
    {
        struct  
        {
        float x, y, z;
        };
        float array[3];
    };

    static const Vector3 cZero;
    static const Vector3 cXAxis;
    static const Vector3 cYAxis;
    static const Vector3 cZAxis;

Vector3();
Vector3(float);
Vector3(float x, float y, float z);
Vector3(Vec2Param vec2, float z = 0.0f);
Vector3(const glm::vec3& rhs);
Vector3(const physx::PxVec3& rhs);
Vector3(const FMOD_VECTOR& rhs);
Vector3(const aiVector3D& rhs);

    // Convert Vector3 to GlmVec3
    glm::vec3 Vec3toGlmVec3();
    physx::PxVec3 Vec3toPVex3();
    FMOD_VECTOR Vec3toFmodVec3();

    //float& operator[](unsigned index);
    float operator[](unsigned index) const;

    //Binary Assignment Operators (reals)
    void operator*=(float rhs);
    void operator/=(float rhs);

    //Binary Operators (Reals)

    //Binary Assignment Operators (vectors)
    void operator+=(const Vector3& rhs);
    void operator-=(const Vector3& rhs);

    //Binary Operators (vectors)

    //Binary Vector Comparisons

    //Vector component wise multiply and divide

    ///Component-wise assignment multiplication
    void operator*=(const Vector3& rhs);
    void operator/=(const Vector3& rhs);


    ///Set all of the values of this vector at once.
    void Set(float x, float y, float z);

    ///Set all of the values of the vector to the passed in value.
    void Splat(float xyz);

    ///Do a component-wise scaling of this vector with the given vector.
    void ScaleByVector(Vec3Param rhs);

    ///Set all of this vector's elements to 0.
    void ZeroOut();

    ///Calculate and return this vector reflected about the given vector.
    Vector3 Reflect(Vec3Param reflectionAxis) const;

    ///Projects the vector onto the axis
    Vector3 Project(Vec3Param axis);

    ///Add a vector multiplied by a scalar to this vector. A commonly done 
    ///operation and this reduces temporaries.
    void AddScaledVector(Vec3Param vector, float scalar);

    ///Compute the dot product of this vector with the given vector.
    float Dot(Vec3Param rhs) const;
    // Dot Product of two vectors.
    static float Dot(Vec3Param lhs, Vec3Param rhs);
    ///Get the length of this vector.
    float Length() const;

    ///Get the squared length of this vector.
    float LengthSq() const;

    ///Calculate and return a unit-length copy of this vector.
    Vector3 Normalized() const;

    ///Make this vector have a length of 1, returns the original length.
    float Normalize();

    ///Flip this vector so it's pointing in the opposite direction.
    Vec3Ref Negate();

    ///Compute the cross product of this vector with the given vector.
    Vector3 Cross(Vec3Param rhs) const;

    // Linearly interpolates between two vectors.
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
    // Linearly interpolates between two vectors without clamping the interpolant
    static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);
    // Projects a vector onto another vector.
    static Vector3 Project(const Vector3& vector, const Vector3& onNormal);
    // Projects a vector onto a plane defined by a normal orthogonal to the plane.
    static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);
    // Moves a point /current/ in a straight line towards a /target/ point.
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);
    // Gradually changes a vector towards a desired goal over time.
    static Vector3 SmoothDamp(const Vector3& current, Vector3& target, Vector3& currentVelocity,
        float smoothTime, float maxSpeed, float deltaTime);
    // Returns the angle in degrees between /from/ and /to/. This is always the smallest
    static float Angle(const Vector3& from, const Vector3& to);
    // Returns a vector that is made from the smallest components of two vectors.
    static Vector3 Min(const Vector3& lhs, const Vector3& rhs);
    // Returns a vector that is made from the largest components of two vectors.
    static Vector3 Max(const Vector3& lhs, const Vector3& rhs);

    static float Distance(const Vec3Param lhs, const Vec3Param rhs);


    static Vector3 ClampMagnitude(const Vector3& vector, float maxLength);

    static void RegisterLua(sol::state& lua);
};

Vector3 Quat_to_Euler(Quat& q);

///Compute the distance between two given vectors.
float Distance(Vec3Param lhs, Vec3Param rhs);

///Compute the dot product of the two given vectors.
float Dot(Vec3Param lhs, Vec3Param rhs);

//Vector component-wise multiply
Vector3 ScaledByVector(Vec3Param lhs, Vec3Param rhs);

//Vector component-wise divide
Vector3 DividedByVector(Vec3Param lhs, Vec3Param rhs);

///Get the length of the given vector.
float Length(Vec3Param vec);

///Get the squared length of the given vector.
float LengthSq(Vec3Param vec);

///Calculate and return a unit-length copy of the given vector.
Vector3 Normalized(Vec3Param vec);

///Make the given vector have a length of 1, returns the original length.
float Normalize(Vec3Ptr vec);

///Compute the cross product of the two given vectors.
Vector3 Cross(Vec3Param lhs, Vec3Param rhs);

///Compute the cross product of the two given vectors for 2d.
///The result is only the z axis of the cross product.
Vector3 Cross2d(Vec3Param lhs, Vec3Param rhs);

///Set all of the given vector's elements to 0.
void ZeroOut(Vec3Ptr vec);

///Flips the given vector so it's pointing in the opposite direction.
void Negate(Vec3Ptr vec);

///Returns a vector pointing in the opposite direction of the given vector.
Vector3 Negated(Vec3Param vec);

///Returns a vector with absolute valued elements of the given vector.
Vector3 Abs(Vec3Param vec);

///Returns the component-wise minimum vector of the two vectors.
Vector3 Min(Vec3Param lhs, Vec3Param rhs);

///Returns the component-wise maximum vector of the two vectors.
Vector3 Max(Vec3Param lhs, Vec3Param rhs);

//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

// Adds two vectors.
Vector3 operator+(const Vector3& a, const Vector3& b);
// Subtracts one vector from another.
Vector3 operator-(const Vector3& a, const Vector3& b);
// Negates a vector.
Vector3 operator-(const Vector3& a);
// Multiplies a vector by a number.
Vector3 operator*(const Vector3& a, float d);
// Multiplies a vector by a number.
Vector3 operator*(float d, const Vector3& a);
// Divides a vector by a number.
Vector3 operator/(const Vector3& a, float d);
Vector3 operator/(const Vector3& a, const Vector3& b);

Vector3 operator*(const Vector3& lhs, const Vector3& rhs);
Vector3 operator*(const Vector3& lhs, float rhs);
Vector3 operator*(float lhs, const Vector3& rhs);

bool operator==(const Vector3& lhs, const Vector3& rhs);
bool operator!=(const Vector3& lhs, const Vector3& rhs);

#endif // HEADER_GUARD