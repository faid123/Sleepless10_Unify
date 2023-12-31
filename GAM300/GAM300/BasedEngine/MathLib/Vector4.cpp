#include "Precompiled.h"
#include <foundation/PxVec4.h>

const Vector4 Vector4::cZero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::cXAxis(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::cYAxis(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::cZAxis(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::cWAxis(0.0f, 0.0f, 0.0f, 1.0f);

Vector4::Vector4()
    :x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{0.0f}
{
}
Vector4::Vector4(float x_)
    : x{ x_ }, y{ x_ }, z{ x_ }, w{x_}
{
}

Vector4::Vector4(float xx, float yy, float zz, float ww)
{
    x = xx;
    y = yy;
    z = zz;
    w = ww;
}

Vector4::Vector4(Vec2Param rhs, float zz, float ww)
{
    x = rhs.x;
    y = rhs.y;
    z = zz;
    w = ww;
}

Vector4::Vector4(Vec3Param rhs, float ww)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = ww;
}

///////////////// Vector 4 ///////////////////////////////
Vector4::Vector4(const glm::vec4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}
Vector4::Vector4(const physx::PxVec4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}

glm::vec4 Vector4::Vec4toGlmVec4()
{
    return glm::vec4{ x,y,z,w };
}

physx::PxVec4 Vector4::Vec4toPVex4()
{
    physx::PxVec4 lhs{ x, y ,z,w };
    return lhs;
}


////////// Binary Assignment Operators (reals) /////////////////////////////////

void Vector4::operator*=(float rhs)
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  w *= rhs;
}

void Vector4::operator/=(float rhs)
{
  if(rhs!= 0.f)
  {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
  }
}

////////// Binary Assignment Operator (Vectors) ////////////////////////////////

void Vector4::operator+=(Vec4Param rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
}

void Vector4::operator-=(Vec4Param rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
}

void Vector4::operator*=(Vec4Param rhs)
{
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  w *= rhs.w;
}

void Vector4::operator/=(Vec4Param rhs)
{
  x /= rhs.x;
  y /= rhs.y;
  z /= rhs.z;
  w /= rhs.w;
}







float& Vector4::operator[](unsigned index)
{
    return ((float*)this)[index];
}

void Vector4::Set(float x_, float y_, float z_, float w_)
{
  x = x_;
  y = y_;
  z = z_;
  w = w_;
}

void Vector4::Splat(float xyzw)
{
  x = y = z = w = xyzw;
}

//Do a component-wise scaling of this vector with the given vector.
void Vector4::ScaleByVector(Vec4Param rhs)
{
  *this *= rhs;
}

Vector4 Vector4::ScaledByVector(Vec4Param rhs) const
{
  return *this * rhs;
}

void Vector4::ZeroOut()
{
  array[0] = 0.0f;
  array[1] = 0.0f;
  array[2] = 0.0f;
  array[3] = 0.0f;
}

void Vector4::AddScaledVector(Vec4Param vector, float scalar)
{
  x += vector.x * scalar;
  y += vector.y * scalar;
  z += vector.z * scalar;
  w += vector.w * scalar;
}

float Vector4::Dot(Vec4Param rhs) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

float Vector4::Length() const
{
  return std::sqrt(LengthSq());
}

float Vector4::LengthSq() const
{
  return Dot(*this);
}

Vector4 Vector4::Normalized() const
{
  Vector4 ret = *this;
  ret /= Length();
  return ret;
}

float Vector4::Normalize()
{
  float length = Length();
  *this /= length;
  return length;
}

Vec4Ref Vector4::Negate()
{
  (*this) *= -1.0f;
  return *this;
}

Vector4 operator*(float lhs, Vec4Param rhs)
{
  return rhs * lhs;
}

float Dot(Vec4Param lhs, Vec4Param rhs)
{
  return lhs.Dot(rhs);
}

float Length(Vec4Param vect)
{
  return vect.Length();
}

float LengthSq(Vec4Param vect)
{
  return vect.LengthSq();
}

Vector4 Normalized(Vec4Param vect)
{
  return vect.Normalized();
}


float Normalize(Vec4Ptr vect)
{
    if (vect)
        return vect->Normalize();
    else return 0.0f;
}


void Negate(Vec4Ptr vec)
{
  if(vec)
    *vec *= -1.0f;
}

Vector4 Negated(Vec4Param vec)
{
  return Vector4(-vec.x, -vec.y, -vec.z, -vec.w);
}

Vector4 Abs(Vec4Param vec)
{
  return Vector4(std::abs(vec.x), std::abs(vec.y), 
                 std::abs(vec.z), std::abs(vec.w));
}

Vector4 Vector4::Min(const Vector4& lhs, const Vector4& rhs)
{
  return Vector4(std::min(lhs.x, rhs.x),
                 std::min(lhs.y, rhs.y),
                 std::min(lhs.z, rhs.z),
                 std::min(lhs.w, rhs.w));
}

Vector4 Vector4::Max(const Vector4& lhs, const Vector4& rhs)
{
  return Vector4(std::max(lhs.x, rhs.x),
                 std::max(lhs.y, rhs.y),
                 std::max(lhs.z, rhs.z),
                 std::max(lhs.w, rhs.w));
}
//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

// Add a vector to another vector
Vector4 operator+(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }

// Subtract a vector to another vector
Vector4 operator-(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }

// Negates a vector.
Vector4 operator-(const Vector4& lhs) { return Vector4(-lhs.x, -lhs.y, -lhs.z, -lhs.w); }

// Is equal to vector
bool operator==(const Vector4& lhs, const Vector4& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }

// Is not equal to vector
bool operator!=(const Vector4& lhs, const Vector4& rhs) { return !(lhs == rhs); }

// Multiplies a vector by a number.
Vector4 operator*(const Vector4& lhs, float rhs) { return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
Vector4 operator*(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }

// Divide a vector by a number.
Vector4 operator/(const Vector4& lhs, float rhs)
{
    Vector4 tmp;
    if (rhs != 0.f)
    {
        return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }
    else
        return tmp;
}

Vector4 operator/(const Vector4& lhs, const Vector4& rhs)
{
    Vector4 tmp;
    if (rhs.x != 0.0f || rhs.y != 0.0f ||
        rhs.z != 0.0f || rhs.w != 0.0f)
        return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
    else return tmp;
}
