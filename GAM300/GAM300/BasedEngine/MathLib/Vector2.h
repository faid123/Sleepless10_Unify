#pragma once

#ifndef VECTOR2_H_
#define VECTOR2_H_
#include <iostream>
#include <glm/glm.hpp>
#include "PxPhysics.h"

struct Vector2;
typedef const Vector2& Vec2Param;
typedef Vector2& Vec2Ref;
typedef Vector2* Vec2Ptr;

struct Vector2
{
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    union
    {
        struct  
        {
        float x, y;
        };
        float array[2];
    };

static const Vector2 cZero;
static const Vector2 cXAxis;
static const Vector2 cYAxis;

Vector2();
explicit Vector2(float x, float y);
Vector2(const glm::vec2& rhs);
//Vector2(const physx::PxVec2& rhs);

glm::vec2 Vec2toGlmVec2();
//physx::PxVec2& Vec2toPVex2();

float& operator[](unsigned index);
float operator[](unsigned index) const;

//Unary Operators
Vector2 operator-() const;

//Binary Assignment Operators (reals)
void operator*=(float rhs);
void operator/=(float rhs);

//Binary Operators (Reals)
Vector2 operator*(float rhs) const;
Vector2 operator/(float rhs) const;

//Binary Assignment Operators (vectors)
void operator+=(Vec2Param rhs);
void operator-=(Vec2Param rhs);

//Binary Operators (vectors)
Vector2 operator+(Vec2Param rhs) const;
Vector2 operator-(Vec2Param rhs) const;

//Binary Vector Comparisons
bool operator==(Vec2Param rhs) const;
bool operator!=(Vec2Param rhs) const;

//Vector component-wise multiply and divide
Vector2 operator*(Vec2Param rhs) const;
Vector2 operator/(Vec2Param rhs) const;

///Component-wise assignment multiplication
void operator*=(Vec2Param rhs);
void operator/=(Vec2Param rhs);

Vector2& operator=(Vector2 Vec2)
{
    if (this != &Vec2)
    {
        this->x = Vec2.x;
        this->y = Vec2.y;
    }

    return *this;
}
///Set all of the values of this vector at once.
void Set(float x, float y);

///Set all of this vector's elements to 0.
void ZeroOut();

///Calculate and return this vector reflected about the given vector.
Vector2 Reflect(Vec2Param reflectionAxis) const;

///Add a vector multiplied by a scalar to this vector. A commonly done 
///operation and this reduces temporaries.
void AddScaledVector(Vec2Param vector, float scalar);

///Compute the dot product of this vector with the given vector.
float Dot(Vec2Param rhs) const;

///Get the length of this vector.
float Length() const;

///Get the squared length of this vector.
float LengthSq() const;

///Calculate and return a unit-length copy of this vector.
Vector2 Normalized() const;

///Make this vector have a length of 1, returns the original length.
float Normalize();

///Attempt to give this vector a length of 1, but checks if it's possible.
///Instead of crashing, will return 0 if the vector was not able to be 
///normalized.
//float AttemptNormalize();

///Flips this vector so it's pointing in the opposite direction.
Vec2Ref Negate();

///Sets all components in vector to value
void Splat(float value);
};

Vector2 operator*(float lhs, Vec2Param rhs);

///Compute the distance between two given points.
float Distance(Vec2Param lhs, Vec2Param rhs);

///Compute the dot product of the two given vectors.
float Dot(Vec2Param lhs, Vec2Param rhs);

///Compute the 2d cross product of the two given vectors.
float Cross(Vec2Param lhs, Vec2Param rhs);

///Get the length of the given vector.
float Length(Vec2Param vec);

///Get the squared length of the given vector.
float LengthSq(Vec2Param vec);

///Calculate and return a unit-length copy of the given vector.
Vector2 Normalized(Vec2Param vec);

///Make the given vector have a length of 1, returns the original length.
float Normalize(Vec2Ptr vec);

///Attempt to give the given vector a length of 1, but checks if it's possible.
///Instead of crashing, this will return 0 if the vector was not able to be 
///normalized.
//float AttemptNormalize(Vec2Ptr vec);

///Flips the given vector so it's pointing in the opposite direction.
void Negate(Vec2Ptr vec);

///Returns a vector pointing in the opposite direction of the given vector.
Vector2 Negated(Vec2Param vec);

///Returns a vector with absolute valued elements of the given vector.
Vector2 Abs(Vec2Param vec);

///Returns the component-wise minimum vector of the two vectors.
Vector2 Min(Vec2Param lhs, Vec2Param rhs);

///Returns the component-wise maximum vector of the two vectors.
Vector2 Max(Vec2Param lhs, Vec2Param rhs);

///Get the perpendicular vector to the given vector
Vector2 GetPerpendicular(Vec2Param vec);

#endif