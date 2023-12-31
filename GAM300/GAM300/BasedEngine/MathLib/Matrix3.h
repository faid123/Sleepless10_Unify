#pragma once

#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "Vector3.h"


struct Matrix3;
typedef const Matrix3& Mat3Param;
typedef Matrix3& Mat3Ref;
typedef Matrix3* Mat3Ptr;

struct Matrix3
{
    union 
    {
        struct
        {
            float m00, m10, m20,
                  m01, m11, m21, 
                  m02, m12, m22;
        };
        struct
        {
            Vector3 vect[3];
        };
        float array[9];
    };
    static const Matrix3 cIdentity;

    Matrix3() {};
    Matrix3(float p00, float p01, float p02, 
                   float p10, float p11, float p12,
                   float p20, float p21, float p22);

    Matrix3(const glm::mat3&);
    Matrix3(const physx::PxMat33&);
    glm::mat3 Mtx3toGlmMat3();
    physx::PxMat33 Mtx3toPxMat3();


    //Binary assignment operators (reals)
    void operator*=(float rhs);
    void operator/=(float rhs);

    //Binary operators (reals)
    Matrix3 operator*(float rhs) const;
    Matrix3 operator/(float rhs) const;

    //Binary assignment operators (matrices)
    void operator+=(Mat3Param rhs);
    void operator-=(Mat3Param rhs);
    
    //Binary operators (matrices)
    Matrix3 operator+(Mat3Param rhs) const;
    Matrix3 operator-(Mat3Param rhs) const;
    Matrix3 operator*(Mat3Param rhs) const;

    //Matrix comparisons
    bool operator==(Mat3Param rhs) const;
    bool operator!=(Mat3Param rhs) const;

    float operator()(unsigned r, unsigned c) const;
    //float& operator()(unsigned r, unsigned c);


    Vector3& operator[](unsigned index);
    const Vector3& operator[](unsigned index) const;


    ///Returns a copy of this matrix with its elements transposed.
    Matrix3 Transposed() const;

    ///Transposes this matrix in place.
    Mat3Ref Transpose();

    ///Returns the inverse of this matrix.
    Matrix3 Inverted() const;

    ///Inverts this matrix in place.
    Matrix3 Invert();

    ///Sets this matrix's elements to that of the identity matrix.
    Mat3Ref SetIdentity();

    ///Sets all of the elements in this matrix to 0.
    Mat3Ref ZeroOut();

    ///Multiplies this matrix with the given matrix on its right-hand side.
    Matrix3 Concat(Mat3Param rhs) const;

    float Determinant() const;

    
    ///Accesses the basis vector at the given index, with the basis vector defined
    ///as the basis vector of a pure rotation matrix.
    Vector3 Basis(unsigned index) const;

    ///Accesses the elements in the "x-axis" of the matrix, with the "x-axis" 
    ///defined as the x-axis of a pure rotation matrix.
    Vector3 BasisX() const;

    ///Accesses the elements in the "y-axis" of the matrix, with the "y-axis" 
    ///defined as the y-axis of a pure rotation matrix.
    Vector3 BasisY() const;

    ///Accesses the elements in the "z-axis" of the matrix, with the "z-axis" 
    ///defined as the z-axis of a pure rotation matrix.
    Vector3 BasisZ() const;

    ///Accesses the cross vector at the given index, with the cross vector defined
    ///as the elements in the matrix perpendicular to that of the corresponding
    ///basis vector.
    Vector3 Cross(unsigned index) const;



    ///Converts this matrix into a pure scaling matrix.
    void Scale(float x, float y, float z);

    ///Converts this matrix into a pure scaling matrix.
    void Scale(Vec3Param rhs);

    ///Converts this matrix into a pure rotation matrix, given an axis-angle pair.
    void Rotate(float x, float y, float z, float degrees);

    ///Converts this matrix into a pure rotation matrix, given an axis-angle pair.
    void Rotate(Vec3Param rhs, float degres);

    ///Converts this matrix into a pure translation matrix for 2-D vectors.
    void Translate(float x, float y);

    ///Converts this matrix into a pure translation matrix for 2-D vectors.
    void Translate(Vec2Param rhs);


};

Matrix3 operator*(float lhs, Mat3Param rhs);
Matrix3 Concat(Mat3Param lhs, Mat3Param rhs);
void Invert(Mat3Ptr matrix);
Matrix3 Inverted(Mat3Param matrix);



#endif