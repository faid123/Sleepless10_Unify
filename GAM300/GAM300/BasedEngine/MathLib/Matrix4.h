#pragma once
#ifndef Matrix4_H
#define Matrix4_H

#include "Precompiled.h"
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>

struct Matrix4;
typedef const Matrix4& Mat4Param;
typedef Matrix4& Mat4Ref;
typedef Matrix4* Mat4Ptr;

struct Matrix4
{
#ifdef _MSC_VER
        // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    union
    {
        struct
        {
            Vector4 _vect[4];
        };
        float _m[4][4];
    };
          
    Matrix4();
    Matrix4(const float* pArr);
    Matrix4(float _00, float _01, float _02, float _03,
        float _10, float _11, float _12, float _13,
        float _20, float _21, float _22, float _23,
        float _30, float _31, float _32, float _33);

    Matrix4(const glm::mat4&);
    Matrix4(const physx::PxMat44&);
    Matrix4(const Matrix4&);
    Matrix4(const aiMatrix4x4& AssimpMatrix);
    Matrix4(const aiMatrix3x3& AssimpMatrix);

    glm::mat4 Mtx4toGlmMat4() const;
    void Mtx4toPxMat4(physx::PxMat44&);

    //Binary assignment operators (reals)
    void operator*=(float rhs);
    void operator/=(float rhs);

    //Binary operators (reals)
    Matrix4 operator*(float rhs) const;
    Matrix4 operator/(float rhs) const;

    //Binary assignment operators (matrices)
    void operator+=(Mat4Param rhs);
    void operator-=(Mat4Param rhs);

    //Binary operators (matrices)
    Matrix4 operator+(Mat4Param rhs) const;
    Matrix4 operator-(Mat4Param rhs) const;
    Matrix4 operator*(Mat4Param rhs) const;
    Vector4 operator*(Vector4& rhs)  const;

    //Matrix comparisons
    bool operator==(Mat4Param rhs) const;
    bool operator!=(Mat4Param rhs) const;

    float operator()(unsigned r, unsigned c) const;
    Vector4& operator[](unsigned index);
    const Vector4& operator[](unsigned index) const;

    Matrix4 Transposed() const;
    Mat4Ref Transpose();//
    Matrix4 Inverted() const;
    Matrix4 Invert();
    Mat4Ref SetIdentity();
    Mat4Ref ZeroOut();

    float Determinant() const;
    Vector4 Basis(unsigned index) const;

    void Scale(float x, float y, float z,float w);
    void Scale(Vec4Param rhs);
    void RotateRad(float angle);
    void RotateDeg(float degres);
    void Translate(float x, float y,float z);
    void Translate(Vec3Param rhs);

    Vector3 getXdir()
    {
        return Vector3(_m[0][0], _m[0][1], _m[0][2]);
    }
    Vector3 getYdir()
    {
        return Vector3(_m[1][0], _m[1][1], _m[1][2]);
    }
    Vector3 getZdir()
    {
        return Vector3(_m[2][0], _m[2][1], _m[2][2]);
    }
    Vector3 getTranslation()
    {
        return Vector3(_m[3][0], _m[3][1], _m[3][2]);
    }
    //special glm decompose
    static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& rhs);
};
void Invert(Mat4Ptr matrix);
Matrix4 Inverted(Mat4Param matrix);

#endif 
