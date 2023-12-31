#include "Precompiled.h"


const Vector2 Vector2::cZero(0.0f, 0.0f);
const Vector2 Vector2::cXAxis(1.0f, 0.0f);
const Vector2 Vector2::cYAxis(0.0f, 1.0f);

Vector2::Vector2()
    :x{ 0.0f }, y{ 0.0f }
{

}

Vector2::Vector2(float x_, float y_)
{
    x = x_;
    y = y_;
}

Vector2::Vector2(const glm::vec2& rhs)
{
    x = rhs.x;
    y = rhs.y;
}
//Vector2::Vector2(const physx::PxVec2& rhs)
//{
//    x = rhs.x;
//    y = rhs.y;
//}

// Convert Vector2D to GlmVec2
glm::vec2 Vector2::Vec2toGlmVec2()
{
    return glm::vec2{ x,y };
}
//physx::PxVec2& Vector2::Vec2toPVex2()
//{
//    physx::PxVec2 lhs{ x, y };
//    return lhs;
//}

float& Vector2::operator[](unsigned index)
{
  return array[index];
}

float Vector2::operator[](unsigned index) const
{
  return array[index];
}


////////// Unary Operators /////////////////////////////////////////////////////

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}


////////// Binary Assignment Operators (reals) /////////////////////////////////

void Vector2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
}

void Vector2::operator/=(float rhs)
{
    if(rhs != 0.f)
    {
        x /= rhs;
        y /= rhs;
    }
}

////////// Binary Operators (reals) ////////////////////////////////////////////

Vector2 Vector2::operator*(float rhs) const
{
    return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator/(float rhs) const
{
    Vector2 tmp;
    if (rhs != 0.f)
    {
        return Vector2(x / rhs, y / rhs);
    }
    else 
        return tmp;
}

////////// Binary Assignment Operators (Vectors) ///////////////////////////////

void Vector2::operator+=(Vec2Param rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vector2::operator-=(Vec2Param rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

////////// Binary Operators (Vectors) //////////////////////////////////////////

Vector2 Vector2::operator+(Vec2Param rhs) const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(Vec2Param rhs) const
{
    return Vector2(x - rhs.x, y - rhs.y);
}

////////// Binary Vector Comparisons ///////////////////////////////////////////

bool Vector2::operator==(Vec2Param rhs) const
{
    return x == rhs.x && 
            y == rhs.y;
}

bool Vector2::operator!=(Vec2Param rhs) const
{
    return !(*this == rhs);
}

////////// Vector component-wise multiply and divide ////////////////////////////

Vector2 Vector2::operator*(Vec2Param rhs) const
{
    return Vector2(x * rhs.x, y * rhs.y);
}

Vector2 Vector2::operator/(Vec2Param rhs) const
{
    Vector2 tmp;
    if (rhs.x != 0.f || rhs.y != 0.f)
        return Vector2(x / rhs.x, y / rhs.y);
    else
        return tmp;
}

////////// Component-wise assignment multiplication ////////////////////////////

void Vector2::operator*=(Vec2Param rhs)
{
    x *= rhs.x;
    y *= rhs.y;
}

void Vector2::operator/=(Vec2Param rhs)
{
    x /= rhs.x;
    y /= rhs.y;
}

////////// Other helper Functions Part 1/////////////////////////////////////////

void Vector2::Set(float x_, float y_)
{
    x = x_;
    y = y_;
}

void Vector2::ZeroOut()
{
    array[0] = 0.0f;
    array[1] = 0.0f;
}


Vector2 Vector2::Reflect(Vec2Param rhs) const
{
    Vector2 reflect = rhs; 
            reflect *= (*this).Dot(rhs);
            reflect *= 2.0f;
            reflect -= *this;
    return reflect;
}

void Vector2::AddScaledVector(Vec2Param vector, float scalar)
{
    x += vector.x * scalar;
    y += vector.y * scalar;
}

float Vector2::Dot(Vec2Param rhs) const
{
    return x * rhs.x + y * rhs.y;
}

float Vector2::Length() const
{
    return std::sqrt(LengthSq());
}

float Vector2::LengthSq() const
{
    return Dot(*this);
}

Vector2 Vector2::Normalized() const
{
    Vector2 ret = *this;
    ret /= Length();
    return ret;
}

float Vector2::Normalize()
{
    float length = Length();
    *this /= length;
    return length;
}

Vec2Ref Vector2::Negate()
{
    (*this) *= -1.0f;
    return *this;
}

void Vector2::Splat(float value)
{
    x = y = value;
}


////////// Other helper Functions Part 2/////////////////////////////////////////

Vector2 operator*(float lhs, Vec2Param rhs)
{
    return rhs * lhs;
}

float Distance(Vec2Param lhs, Vec2Param rhs)
{
    return (rhs - lhs).Length();
}

float Dot(Vec2Param lhs, Vec2Param rhs)
{
    return lhs.Dot(rhs);
}

float Cross(Vec2Param lhs, Vec2Param rhs)
{
    return lhs.x * rhs.y - rhs.x * lhs.y;
}

float Length(Vec2Param vect)
{
    return vect.Length();
}

float LengthSq(Vec2Param vect)
{
    return vect.LengthSq();
}

Vector2 Normalized(Vec2Param vect)
{
    return vect.Normalized();
}

float Normalize(Vec2Ptr vect)
{
    if (vect)
        return vect->Normalize();
    else return 0.0f;
}

Vector2 Negated(Vec2Param vec)
{
    return Vector2(-vec.x, -vec.y);
}

Vector2 Abs(Vec2Param vec)
{
    return Vector2(std::abs(vec.x), std::abs(vec.y));
}

Vector2 Min(Vec2Param lhs, Vec2Param rhs)
{
    return Vector2(std::min(lhs.x, rhs.x),
                    std::min(lhs.y, rhs.y));
}

Vector2 Max(Vec2Param lhs, Vec2Param rhs)
{
    return Vector2(std::max(lhs.x, rhs.x),
                    std::max(lhs.y, rhs.y));
}

Vector2 GetPerpendicular(Vec2Param vec)
{
    return Vector2(vec.y, -vec.x);
}