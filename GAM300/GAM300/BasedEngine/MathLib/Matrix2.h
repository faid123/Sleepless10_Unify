#pragma once

#ifndef MATRIX2_H_
#define MATRIX2_H_

#include "Vector2.h"

struct Matrix2;
typedef const Matrix2& Mat2Param;
typedef Matrix2& Mat2Ref;

struct Matrix2
{
    union 
    {
        struct
        {
            float m00, m01, 
                m10, m11;
        };
        struct 
        {
            Vector2 vect[2];
        };

        float array[4];
    };


    Matrix2() {};
    Vec2Ref operator[](unsigned index);
    Vec2Param operator[](unsigned index) const;
    const float* Begin() const;
    const float* End() const;
    float* Begin();
    float* End();

    //binary assignment operators (Reals)
    void operator*=(float rhs);
    void operator/=(float rhs);

    //binary operators (Reals)
    Matrix2 operator*(float rhs) const;
    Matrix2 operator/(float rhs) const;

    //binary assignment operators (matrices)
    void operator+=(Mat2Param rhs);
    void operator-=(Mat2Param rhs);

    //binary operators (matrices)
    Matrix2 operator+(Mat2Param rhs) const;
    Matrix2 operator-(Mat2Param rhs) const;
    Matrix2 operator*(Mat2Param rhs) const;

    //matrix comparisons
    bool operator==(Mat2Param rhs) const;
    bool operator!=(Mat2Param rhs) const;

    Matrix2 Concat(Mat2Param rhs) const;
    Matrix2 Transpose() const;
    Matrix2 Inverse();
    Mat2Ref Identity();
    Mat2Ref Zero();
    
    float Determinate() const;

    void Scale(float x, float y);
    void Scale(Vec2Param rhs);
    Vector2 Transform(Vec2Param vector) const;
    void TransformVector(Vec2Ref vector) const;

    Vector2 BasisVector(unsigned index) const;
};

Matrix2 operator*(float lhs, Mat2Param rhs);

Matrix2 Concat(Mat2Param lhs, Mat2Param rhs);
Vector2 Transform(Mat2Param mat, Vec2Param vector);


#endif