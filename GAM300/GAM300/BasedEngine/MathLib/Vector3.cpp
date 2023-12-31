#include "Vector3.h"
#include "Precompiled.h"
#include <corecrt_math_defines.h>


static constexpr float Deg2Rad = 3.14159265358979323846264338327950288f * 2.0f / 360.0f;
static constexpr float Rad2Deg = 1.0f / Deg2Rad;

const Vector3 Vector3::cZero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::cXAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::cYAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::cZAxis(0.0f, 0.0f, 1.0f);

inline float Clamp01(float value) {
    if (value < 0.f)
        return 0.f;
    else if (value > 1.f)
        return 1.f;
    else
        return value;
}

inline float Clamp(float value, float min, float max) {
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}


Vector3::Vector3()
    :x{ 0.0f }, y{ 0.0f }, z{0.0f}
{

}
Vector3::Vector3(float x_)
    : x{ x_ }, y{ x_ }, z{ x_ }
{

}

Vector3::Vector3(float xx, float yy, float zz) : x{ xx }, y{ yy }, z{ zz }
{
}

Vector3::Vector3(Vec2Param rhs, float zz)
{
    x = rhs.x;
    y = rhs.y;
    z = zz;
}

///////////////// Vector 3 ///////////////////////////////
Vector3::Vector3(const glm::vec3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}
Vector3::Vector3(const physx::PxVec3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

Vector3::Vector3(const FMOD_VECTOR& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}
Vector3::Vector3(const aiVector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

// Convert Vector2D to GlmVec2
glm::vec3 Vector3::Vec3toGlmVec3()
{
    return glm::vec3{ x,y,z };
}
physx::PxVec3 Vector3::Vec3toPVex3()
{
    physx::PxVec3 lhs{ x, y ,z };
    return lhs;
}

FMOD_VECTOR Vector3::Vec3toFmodVec3()
{
    FMOD_VECTOR vector = { x, y, z };
    return vector;
}

//float& Vector3::operator[](unsigned index)
//{
//    float tmp;
//    if(index <=2)
//        tmp = array[index];
//    return tmp;
//}

float Vector3::operator[](unsigned index) const
{
    if (index <= 2)
        return array[index];
    else
        return 0.0f;
}

////////// Unary Operators /////////////////////////////////////////////////////


////////// Binary Assignment Operators (reals) /////////////////////////////////

void Vector3::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
}

void Vector3::operator/=(float rhs)
{
    if(rhs != 0.f)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
    }
}

////////// Binary Operators (reals) ////////////////////////////////////////////

////////// Binary Assignment Operators (Vectors) ///////////////////////////////

void Vector3::operator+=(const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

void Vector3::operator-=(const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
}

////////// Binary Operators (Vectors) //////////////////////////////////////////

////////// Binary Vector Comparisons ///////////////////////////////////////////


////////// Vector component-wise multiply and divide ////////////////////////////


////////// Component-wise assignment multiplication ////////////////////////////
void Vector3::operator*=(const Vector3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
}

void Vector3::operator/=(const Vector3& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
}

////////// Other helper Functions Part 1/////////////////////////////////////////

void Vector3::Set(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
}

void Vector3::Splat(float xyz)
{
    x = y = z = xyz;
}

void Vector3::ScaleByVector(const Vector3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
}

void Vector3::ZeroOut()
{
    array[0] = 0.0f;
    array[1] = 0.0f;
    array[2] = 0.0f;
}

Vector3 Vector3::Reflect(Vec3Param rhs) const
{
    Vector3 reflect  = rhs;
            reflect *= (*this).Dot(rhs);
            reflect *= 2.0f;
            reflect -= *this;
    return reflect;
}

Vector3 Vector3::Project(Vec3Param axis)
{
    float dot = Dot(axis);
    return axis * dot;
}

void Vector3::AddScaledVector(Vec3Param vector, float scalar)
{
    x += vector.x * scalar;
    y += vector.y * scalar;
    z += vector.z * scalar;
}

float Vector3::Dot(Vec3Param rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Vector3::Dot(Vec3Param lhs, Vec3Param rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;;
}

float Vector3::Length() const
{
    return std::sqrt(LengthSq());
}

float Vector3::LengthSq() const
{
    return Dot(*this);
}

Vector3 Vector3::Normalized() const
{
    Vector3 ret = *this;
    ret /= Length();
    return ret;
}

float Vector3::Normalize()
{
    float length = Length();
    *this /= length;
    return length;
}

Vec3Ref Vector3::Negate()
{
    (*this) *= -1.0f;
    return *this;
}

Vector3 Vector3::Cross(Vec3Param rhs) const
{
    Vector3 ret;
    ret.x = y * rhs.z - z * rhs.y;
    ret.y = z * rhs.x - x * rhs.z;
    ret.z = x * rhs.y - y * rhs.x;
    return ret;
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
{
    t = Clamp01(t);
    return Vector3{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,a.z + (b.z - a.z) * t };
}

Vector3 Vector3::LerpUnclamped(const Vector3& a, const Vector3& b, float t)
{
    return Vector3{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,a.z + (b.z - a.z) * t };
}

Vector3 Vector3::Project(const Vector3& vector, const Vector3& onNormal)
{
    float sqrMag = Dot(onNormal, onNormal);
    if (sqrMag < /*Mathf.Epsilon*/0.00001f)
        return Vector3{};
    else
        return onNormal * Dot(vector, onNormal) / sqrMag;
}

Vector3 Vector3::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
{
    return vector - Project(vector, planeNormal);
}

Vector3 Vector3::MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta)
{
    Vector3 toVector = target - current;
    float dist = toVector.Length();
    if (dist <= maxDistanceDelta || dist < /*float.Epsilon*/ 0.0001f)
        return target;
    return current + toVector / dist * maxDistanceDelta;
}

Vector3 Vector3::SmoothDamp(const Vector3& current, Vector3& target, Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    smoothTime = std::max(0.0001F, smoothTime);
    float omega = 2.0f / smoothTime;

    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48F * x * x + 0.235F * x * x * x);
    Vector3 change = current - target;
    Vector3 originalTo = target;

    float maxChange = maxSpeed * smoothTime;
    change = Vector3::ClampMagnitude(change, maxChange);
    target = current - change;

    Vector3 temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - omega * temp) * exp;
    Vector3 output = target + (change + temp) * exp;

    if (Vector3::Dot(originalTo - current, output - originalTo) > 0)
    {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}

Vector3 Vector3::ClampMagnitude(const Vector3& vector, float maxLength)
{
    if (vector.LengthSq() > maxLength * maxLength)
        return vector.Normalized() * maxLength;
    return vector;
}

float Vector3::Angle(const Vector3& from, const Vector3& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    float denominator = std::sqrtf(from.LengthSq() * to.LengthSq());
    if (denominator < 0.00001f)
        return 0.0f;

    float dot = Clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    return std::acosf(dot) * Rad2Deg;
}

Vector3 Vector3::Min(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

Vector3 Vector3::Max(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
}

float Vector3::Distance(const Vec3Param lhs, const Vec3Param rhs)
{   
    Vector3 dist = rhs - lhs;
    return dist.Length();
}


////////// Other helper Functions Part 2/////////////////////////////////////////

Vector3 Quat_to_Euler(Quat& q)
{
    // roll (x-axis rotation)
    float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    float x = std::atan2f(sinr_cosp, cosr_cosp) * (180.0f / (float)M_PI);

    // pitch (y-axis rotation)
    float sinp = 2.0f * (q.w * q.y - q.z * q.x);
    float y;
    if (std::abs(sinp) >= 1.0f)
        y = std::copysignf((float)M_PI / 2, sinp) * (180.0f / (float)M_PI); // use 90 degrees if out of range
    else
        y = std::asinf(sinp) * (180.0f / (float)M_PI);

    // yaw (z-axis rotation)
    float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    float z = std::atan2f(siny_cosp, cosy_cosp) * (180.0f / (float)M_PI);

    return Vector3(x, y, z);
}


float Distance(Vec3Param lhs, Vec3Param rhs)
{
    return Length(rhs - lhs);
}

float Dot(Vec3Param lhs, Vec3Param rhs)
{  
    return lhs.Dot(rhs);
}

Vector3 ScaledByVector(Vec3Param lhs, Vec3Param rhs)
{
    return lhs * rhs;
}

Vector3 DividedByVector(Vec3Param lhs, Vec3Param rhs)
{
    return lhs / rhs;
}

float Length(Vec3Param vect)
{
    return vect.Length();
}

float LengthSq(Vec3Param vect)
{
    return vect.LengthSq();
}

Vector3 Normalized(Vec3Param vect)
{
    return vect.Normalized();
}

float Normalize(Vec3Ptr vect)
{
    if (vect)
        return vect->Normalize();
    else return 0.0f;
}

Vector3 Cross(Vec3Param lhs, Vec3Param rhs)
{
    return lhs.Cross(rhs);
}

Vector3 Cross2d(Vec3Param lhs, Vec3Param rhs)
{
    Vector3 result = Vector3::cZero;
    result.z = lhs.x * rhs.y - rhs.x * lhs.y;
    return result;
}


void ZeroOut(Vec3Ptr vec)
{
    if(vec)
        vec->ZeroOut();
}

void Negate(Vec3Ptr vec)
{
    if(vec)
        *vec *= -1.0f;
}

Vector3 Negated(Vec3Param vec)
{
    return Vector3(-vec.x, -vec.y, -vec.z);
}

Vector3 Abs(Vec3Param vec)
{
    return Vector3(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z));
}

Vector3 Min(Vec3Param lhs, Vec3Param rhs)
{
    return Vector3(std::min(lhs.x, rhs.x),
                    std::min(lhs.y, rhs.y),
                    std::min(lhs.z, rhs.z));
}

Vector3 Max(Vec3Param lhs, Vec3Param rhs)
{
    return Vector3(std::max(lhs.x, rhs.x),
                    std::max(lhs.y, rhs.y),
                    std::max(lhs.z, rhs.z));
}

//================================================================================
//  OPERATOR OVERLOADING
//================================================================================

// Adds two vectors.
Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3{ a.x + b.x, a.y + b.y, a.z + b.z }; }
// Subtracts one vector from another.
Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3{ a.x - b.x, a.y - b.y, a.z - b.z }; }
// Negates a vector.
Vector3 operator-(const Vector3& a) { return Vector3{ -a.x, -a.y, -a.z }; }
// Multiplies a vector by a number.
Vector3 operator*(const Vector3& a, float d) { return Vector3{ a.x * d, a.y * d, a.z * d }; }
// Multiplies a vector by a number.
Vector3 operator*(float d, const Vector3& a) { return Vector3{ a.x * d, a.y * d, a.z * d }; }
Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}
// Divides a vector by a number.
Vector3 operator/(const Vector3& a, float d)
{
    if (d != 0.0f) // TODO: use epsilon
        return Vector3{ a.x / d, a.y / d, a.z / d };
    else
        return Vector3{};
}

Vector3 operator/(const Vector3& a, const Vector3& b)
{
    if (b.x != 0.0f && b.y != 0.0f && b.z != 0.0f) // TODO: use epsilon
        return Vector3{ a.x / b.x, a.y / b.y, a.z / b.z };
    else
        return Vector3{};
}

bool operator==(const Vector3& lhs, const Vector3& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
bool operator!=(const Vector3& lhs, const Vector3& rhs) { return !(lhs == rhs); }