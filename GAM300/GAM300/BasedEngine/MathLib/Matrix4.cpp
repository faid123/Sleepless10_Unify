#include "Precompiled.h"
#include "Matrix4.h"
#include <foundation/PxMat44.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            _m[i][j] = 0.0f;
}
Matrix4::Matrix4(const float* pArr)
{
    for (int i = 0; i < 4; ++i)
        for (int k = 0; k < 4; ++k)
            _m[i][k] = *(pArr + i + k);
}

Matrix4::Matrix4(float _00, float _01, float _02, float _03,
    float _10, float _11, float _12, float _13,
    float _20, float _21, float _22, float _23,
    float _30, float _31, float _32, float _33)
{
    _m[0][0] = _00;
    _m[0][1] = _01;
    _m[0][2] = _02;
    _m[0][3] = _03;
    _m[1][0] = _10;
    _m[1][1] = _11;
    _m[1][2] = _12;
    _m[1][3] = _13;
    _m[2][0] = _20;
    _m[2][1] = _21;
    _m[2][2] = _22;
    _m[2][3] = _23;
    _m[3][0] = _30;
    _m[3][1] = _31;
    _m[3][2] = _32;
    _m[3][3] = _33;
}

Matrix4::Matrix4(const glm::mat4& rhs)
{
    _m[0][0] = rhs[0][0];
    _m[0][1] = rhs[0][1];
    _m[0][2] = rhs[0][2];
    _m[0][3] = rhs[0][3];
    _m[1][0] = rhs[1][0];
    _m[1][1] = rhs[1][1];
    _m[1][2] = rhs[1][2];
    _m[1][3] = rhs[1][3];
    _m[2][0] = rhs[2][0];
    _m[2][1] = rhs[2][1];
    _m[2][2] = rhs[2][2];
    _m[2][3] = rhs[2][3];
    _m[3][0] = rhs[3][0];
    _m[3][1] = rhs[3][1];
    _m[3][2] = rhs[3][2];
    _m[3][3] = rhs[3][3];
}

Matrix4::Matrix4(const Matrix4& rhs)
{
    for (int i = 0; i < 4; ++i)
        for (int k = 0; k < 4; ++k)
            _m[i][k] = rhs._m[k][i];
}

glm::mat4 Matrix4::Mtx4toGlmMat4() const
{
    glm::mat4 tmp;

    tmp[0][0] = _m[0][0];
    tmp[0][1] = _m[0][1];
    tmp[0][2] = _m[0][2];
    tmp[0][3] = _m[0][3];
    tmp[1][0] = _m[1][0];
    tmp[1][1] = _m[1][1];
    tmp[1][2] = _m[1][2];
    tmp[1][3] = _m[1][3];
    tmp[2][0] = _m[2][0];
    tmp[2][1] = _m[2][1];
    tmp[2][2] = _m[2][2];
    tmp[2][3] = _m[2][3];
    tmp[3][0] = _m[3][0];
    tmp[3][1] = _m[3][1];
    tmp[3][2] = _m[3][2];
    tmp[3][3] = _m[3][3];

    glm::mat4 tmp2 = glm::transpose(tmp);
    return tmp2;
}

///////////////// Matrix 4x4 ///////////////////////////////
void Matrix4::Mtx4toPxMat4(physx::PxMat44& rhs)
{
    rhs[0][0] = _m[0][0];
    rhs[0][1] = _m[0][1];
    rhs[0][2] = _m[0][2];
    rhs[0][3] = _m[0][3];
    rhs[1][0] = _m[1][0];
    rhs[1][1] = _m[1][1];
    rhs[1][2] = _m[1][2];
    rhs[1][3] = _m[1][3];
    rhs[2][0] = _m[2][0];
    rhs[2][1] = _m[2][1];
    rhs[2][2] = _m[2][2];
    rhs[2][3] = _m[2][3];
    rhs[3][0] = _m[3][0];
    rhs[3][1] = _m[3][1];
    rhs[3][2] = _m[3][2];
    rhs[3][3] = _m[3][3];
}

Matrix4::Matrix4(const physx::PxMat44& rhs)
{
    _m[0][0] = rhs[0][1];
    _m[0][1] = rhs[0][1];
    _m[0][2] = rhs[0][2];
    _m[0][3] = rhs[0][3];
    _m[1][0] = rhs[1][0];
    _m[1][1] = rhs[1][1];
    _m[1][2] = rhs[1][2];
    _m[1][3] = rhs[1][3];
    _m[2][0] = rhs[2][0];
    _m[2][1] = rhs[2][1];
    _m[2][2] = rhs[2][2];
    _m[2][3] = rhs[2][3];
    _m[3][0] = rhs[3][0];
    _m[3][1] = rhs[3][1];
    _m[3][2] = rhs[3][2];
    _m[3][3] = rhs[3][3];
}
Matrix4::Matrix4(const aiMatrix4x4& AssimpMatrix)
{
    _m[0][0] = AssimpMatrix.a1; _m[0][1] = AssimpMatrix.a2; _m[0][2] = AssimpMatrix.a3; _m[0][3] = AssimpMatrix.a4;
    _m[1][0] = AssimpMatrix.b1; _m[1][1] = AssimpMatrix.b2; _m[1][2] = AssimpMatrix.b3; _m[1][3] = AssimpMatrix.b4;
    _m[2][0] = AssimpMatrix.c1; _m[2][1] = AssimpMatrix.c2; _m[2][2] = AssimpMatrix.c3; _m[2][3] = AssimpMatrix.c4;
    _m[3][0] = AssimpMatrix.d1; _m[3][1] = AssimpMatrix.d2; _m[3][2] = AssimpMatrix.d3; _m[3][3] = AssimpMatrix.d4;
}

Matrix4::Matrix4(const aiMatrix3x3& AssimpMatrix)
{
    _m[0][0] = AssimpMatrix.a1; _m[0][1] = AssimpMatrix.a2; _m[0][2] = AssimpMatrix.a3; _m[0][3] = 0.0f;
    _m[1][0] = AssimpMatrix.b1; _m[1][1] = AssimpMatrix.b2; _m[1][2] = AssimpMatrix.b3; _m[1][3] = 0.0f;
    _m[2][0] = AssimpMatrix.c1; _m[2][1] = AssimpMatrix.c2; _m[2][2] = AssimpMatrix.c3; _m[2][3] = 0.0f;
    _m[3][0] = 0.0f; _m[3][1] = 0.0f; _m[3][2] = 0.0f; _m[3][3] = 1.0f;
}

/////// Binary Assignment Operators ///////////
void Matrix4::operator*=(float rhs)
{
    _vect[0] *= rhs;
    _vect[1] *= rhs;
    _vect[2] *= rhs;
    _vect[3] *= rhs;
}

void Matrix4::operator/=(float rhs)
{
    if (rhs)
    {
        _vect[0] /= rhs;
        _vect[1] /= rhs;
        _vect[2] /= rhs;
        _vect[3] /= rhs;
    }
}

//////////////// Binary Operators //////////////
Matrix4 Matrix4::operator*(float rhs) const
{
    Matrix4 ret = *this;
    ret *= rhs;
    return ret;
}

Matrix4 Matrix4::operator/(float rhs) const
{
    Matrix4 ret;
    if (rhs)
    {
        ret = *this;
        ret /= rhs;
    }
    return ret;
}

//////// Binary Assignment Operator ///////////////
void Matrix4::operator+=(Mat4Param rhs)
{
    _vect[0] += rhs._vect[0];
    _vect[1] += rhs._vect[1];
    _vect[2] += rhs._vect[2];
    _vect[3] += rhs._vect[3];
}

void Matrix4::operator-=(Mat4Param rhs)
{
    _vect[0] -= rhs._vect[0];
    _vect[1] -= rhs._vect[1];
    _vect[2] -= rhs._vect[2];
    _vect[3] -= rhs._vect[3];
}

//////// Binary Operators ///////////////////////
Matrix4 Matrix4::operator+(Mat4Param rhs) const
{
    Matrix4 ret = *this;
    ret += rhs;
    return ret;
}

Matrix4 Matrix4::operator-(Mat4Param rhs) const
{
    Matrix4 ret = *this;
    ret -= rhs;
    return ret;
}

Matrix4 Matrix4::operator*(Mat4Param rhs) const
{
    Matrix4 temp; 

    temp._m[0][0] = _m[0][0] * rhs._m[0][0] + _m[0][1] * rhs._m[1][0] + _m[0][2] * rhs._m[2][0] + _m[0][3] * rhs._m[3][0];
    temp._m[0][1] = _m[0][0] * rhs._m[0][1] + _m[0][1] * rhs._m[1][1] + _m[0][2] * rhs._m[2][1] + _m[0][3] * rhs._m[3][1];
    temp._m[0][2] = _m[0][0] * rhs._m[0][2] + _m[0][1] * rhs._m[1][2] + _m[0][2] * rhs._m[2][2] + _m[0][3] * rhs._m[3][2];
    temp._m[0][3] = _m[0][0] * rhs._m[0][3] + _m[0][1] * rhs._m[1][3] + _m[0][2] * rhs._m[2][3] + _m[0][3] * rhs._m[3][3];
  
    temp._m[1][0] = _m[1][0] * rhs._m[0][0] + _m[1][1] * rhs._m[1][0] + _m[1][2] * rhs._m[2][0] + _m[1][3] * rhs._m[3][0];
    temp._m[1][1] = _m[1][0] * rhs._m[0][1] + _m[1][1] * rhs._m[1][1] + _m[1][2] * rhs._m[2][1] + _m[1][3] * rhs._m[3][1];
    temp._m[1][2] = _m[1][0] * rhs._m[0][2] + _m[1][1] * rhs._m[1][2] + _m[1][2] * rhs._m[2][2] + _m[1][3] * rhs._m[3][2];
    temp._m[1][3] = _m[1][0] * rhs._m[0][3] + _m[1][1] * rhs._m[1][3] + _m[1][2] * rhs._m[2][3] + _m[1][3] * rhs._m[3][3];
   
    temp._m[2][0] = _m[2][0] * rhs._m[0][0] + _m[2][1] * rhs._m[1][0] + _m[2][2] * rhs._m[2][0] + _m[2][3] * rhs._m[3][0];
    temp._m[2][1] = _m[2][0] * rhs._m[0][1] + _m[2][1] * rhs._m[1][1] + _m[2][2] * rhs._m[2][1] + _m[2][3] * rhs._m[3][1];
    temp._m[2][2] = _m[2][0] * rhs._m[0][2] + _m[2][1] * rhs._m[1][2] + _m[2][2] * rhs._m[2][2] + _m[2][3] * rhs._m[3][2];
    temp._m[2][3] = _m[2][0] * rhs._m[0][3] + _m[2][1] * rhs._m[1][3] + _m[2][2] * rhs._m[2][3] + _m[2][3] * rhs._m[3][3];
  
    temp._m[3][0] = _m[3][0] * rhs._m[0][0] + _m[3][1] * rhs._m[1][0] + _m[3][2] * rhs._m[2][0] + _m[3][3] * rhs._m[3][0];
    temp._m[3][1] = _m[3][0] * rhs._m[0][1] + _m[3][1] * rhs._m[1][1] + _m[3][2] * rhs._m[2][1] + _m[3][3] * rhs._m[3][1];
    temp._m[3][2] = _m[3][0] * rhs._m[0][2] + _m[3][1] * rhs._m[1][2] + _m[3][2] * rhs._m[2][2] + _m[3][3] * rhs._m[3][2];
    temp._m[3][3] = _m[3][0] * rhs._m[0][3] + _m[3][1] * rhs._m[1][3] + _m[3][2] * rhs._m[2][3] + _m[3][3] * rhs._m[3][3];

    return temp;
}

Vector4 Matrix4::operator*(Vector4& rhs) const
{
    Vector4 temp;
    temp.x = _vect[0].x * rhs.x + _vect[0].y * rhs.y + _vect[0].z * rhs.z + _vect[0].w * rhs.w;
    temp.y = _vect[1].x * rhs.x + _vect[1].y * rhs.y + _vect[1].z * rhs.z + _vect[1].w * rhs.w;
    temp.z = _vect[2].x * rhs.x + _vect[2].y * rhs.y + _vect[2].z * rhs.z + _vect[2].w * rhs.w;
    temp.w = _vect[3].x * rhs.x + _vect[3].y * rhs.y + _vect[3].z * rhs.z + _vect[3].w * rhs.w;
	return temp;
}

/////////// Binary Comparisons /////////////////
bool Matrix4::operator==(Mat4Param rhs) const
{
    return _vect[0] == rhs._vect[0] &&
        _vect[1] == rhs._vect[1] &&
        _vect[2] == rhs._vect[2] &&
        _vect[3] == rhs._vect[3];
}

bool Matrix4::operator!=(Mat4Param rhs) const
{
    return !(*this == rhs);
}

float Matrix4::operator()(unsigned r, unsigned c) const
{
    float tmp = 0.0f;
    if (r <= 3 && c <= 3)
    {
        tmp = _m[r][c];
    }
    return tmp;
}


Vector4& Matrix4::operator[](unsigned index)
{
    return ((Vector4*)this)[index];
}

const Vector4& Matrix4::operator[](unsigned index) const
{
    return ((Vector4*)this)[index];
}


Matrix4 Matrix4::Transposed() const
{
    Matrix4 pResult;
    pResult._m[0][1] = _m[1][0];
    pResult._m[1][0] = _m[0][1];
    pResult._m[0][3] = _m[3][0];
    pResult._m[3][0] = _m[0][3];
    pResult._m[0][0] = _m[0][0];

    pResult._m[2][0] = _m[0][2];
    pResult._m[0][2] = _m[2][0];
    pResult._m[2][3] = _m[3][2];
    pResult._m[3][2] = _m[2][3];
    pResult._m[1][1] = _m[1][1];

    pResult._m[2][1] = _m[1][2];
    pResult._m[1][2] = _m[2][1];
    pResult._m[2][2] = _m[2][2];

    pResult._m[3][1] = _m[1][3];
    pResult._m[1][3] = _m[3][1];
    pResult._m[3][3] = _m[3][3];
    return pResult;
}

Mat4Ref Matrix4::Transpose()
{
    std::swap(_m[0][1], _m[1][0]);
    std::swap(_m[0][2], _m[2][0]);
    std::swap(_m[0][3], _m[3][0]);
    std::swap(_m[2][3], _m[3][2]);
    std::swap(_m[2][1], _m[1][2]);
    std::swap(_m[1][3], _m[3][1]);
    return *this;
}

Matrix4 Matrix4::Inverted() const
{
    Matrix4 foo(*this);
    foo.Invert();
    return foo;
}

Matrix4 Matrix4::Invert()
{
    float determinant = Determinant();
    Matrix4 pResult;
    if (determinant != 0.0f)
    {
        Matrix4 temp;
        temp = *this; 
        pResult = (*this).Transposed();
        temp = pResult;

        //Find the determinant of each of the 2x2 minor matrices
        pResult._m[0][0] = temp._m[1][1] * temp._m[2][2] - temp._m[2][1] * temp._m[1][2];
        pResult._m[0][1] = temp._m[1][0] * temp._m[2][2] - temp._m[2][0] * temp._m[1][2];
        pResult._m[0][2] = temp._m[1][0] * temp._m[2][1] - temp._m[2][0] * temp._m[1][1];
        pResult._m[1][0] = temp._m[0][1] * temp._m[2][2] - temp._m[2][1] * temp._m[0][2];
        pResult._m[1][1] = temp._m[0][0] * temp._m[2][2] - temp._m[2][0] * temp._m[0][2];

        pResult._m[1][2] = temp._m[0][0] * temp._m[2][1] - temp._m[2][0] * temp._m[0][1];

        pResult._m[2][0] = temp._m[0][1] * temp._m[1][2] - temp._m[1][1] * temp._m[0][2];
        pResult._m[2][1] = temp._m[0][0] * temp._m[1][2] - temp._m[1][0] * temp._m[0][2];
        pResult._m[2][2] = temp._m[0][0] * temp._m[1][1] - temp._m[1][0] * temp._m[0][1];

        //Create the matrix of cofactors.
        pResult._m[0][0] /= determinant;
        pResult._m[0][1] /= -determinant;
        pResult._m[0][2] /= determinant;

        pResult._m[1][0] /= -determinant;
        pResult._m[1][1] /= determinant;
        pResult._m[1][2] /= -determinant;

        pResult._m[2][0] /= determinant;
        pResult._m[2][1] /= -determinant;
        pResult._m[2][2] /= determinant;
    }
    return pResult;
}

Mat4Ref Matrix4::SetIdentity()
{
    _vect[0].Set(1.0f, 0.0f, 0.0f,0.0f);
    _vect[1].Set(0.0f, 1.0f, 0.0f,0.0f);
    _vect[2].Set(0.0f, 0.0f, 1.0f,0.0f);
    _vect[3].Set(0.0f, 0.0f, 0.0f,1.0f);
    return *this;
}

Mat4Ref Matrix4::ZeroOut()
{
    _vect[0].ZeroOut();
    _vect[1].ZeroOut();
    _vect[2].ZeroOut();
    _vect[3].ZeroOut();
    return *this;
}


float Matrix4::Determinant() const
{
    float FirstMinor = _m[0][0] * (_m[1][1] * _m[2][2] - _m[2][1] * _m[1][2]);
    float SecondMinor = -_m[0][1] * (_m[1][0] * _m[2][2] - _m[2][0] * _m[1][2]);
    float ThirdMinor = _m[0][2] * (_m[1][0] * _m[2][1] - _m[2][0] * _m[1][1]);
    return FirstMinor + SecondMinor + ThirdMinor;
}

Vector4 Matrix4::Basis(unsigned index) const
{
    Vector4 tmp;
    if (index <= 4)
        tmp = (_vect[index]);
    return tmp;
}

void Matrix4::Scale(float x, float y, float z,float w)
{
    SetIdentity();
    _m[0][0] = x;
    _m[1][1] = y;
    _m[2][2] = z;
    _m[3][3] = w;
}


void Matrix4::Scale(Vec4Param rhs)
{
    Scale(rhs.x, rhs.y, rhs.z,rhs.w);
}

void Matrix4::RotateRad(float angle)
{
    _m[0][0] = cos(angle);
    _m[0][1] = -sin(angle);
    _m[0][2] = 0.0f;
    _m[0][3] = 0.0f;

    _m[1][0] = sin(angle);
    _m[1][1] = cos(angle);
    _m[1][2] = 0.0f;
    _m[1][3] = 0.0f;

    _m[2][0] = 0.0f;
    _m[2][1] = 0.0f;
    _m[2][2] = 1.0f;
    _m[2][3] = 0.0f;

    _m[3][0] = 0.0f;
    _m[3][1] = 0.0f;
    _m[3][2] = 0.0f;
    _m[3][3] = 1.0f;
}

void Matrix4::RotateDeg(float degrees)
{
    // Converting it to Radians
    float angleRad = static_cast<float>(degrees * M_PI / 180.f);
    RotateRad(angleRad);
}

void Matrix4::Translate(float x, float y,float z)
{
    _m[0][0] = 1.0f;
    _m[0][1] = 0.0f;
    _m[0][2] = 0.0f; 
    _m[0][3] = x;

    _m[1][0] = 0.0f;
    _m[1][1] = 1.0f;
    _m[1][2] = 0.0f;
    _m[1][3] = y;

    _m[2][0] = 0.0f;
    _m[2][1] = 0.0f;
    _m[2][2] = 1.0f;
    _m[2][3] = z;

    _m[3][0] = 0.0f;
    _m[3][1] = 0.0f;
    _m[3][2] = 0.0f;
    _m[3][3] = 1.0f;
}

void Matrix4::Translate(Vec3Param rhs)
{
    Translate(rhs.x, rhs.y,rhs.z);
}

bool Matrix4::DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& Orientation, glm::vec3& outScale)
{
    // From glm::decompose in matrix_decompose.inl

    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
        return false;

    // First, isolate perspective.  This is the messiest.
    if (
        epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }
    // Next take care of translation (easy).
    outTranslation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3];

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor.
    outScale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));

    // Now, compute Y scale.
    outScale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));

    // Next, get Z scale and normalize 3rd row.
    outScale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0)
    {
        for (length_t i = 0; i < 3; i++)
        {
            Scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
#endif
    //Orientation is Eular
    Orientation.y = asin(-Row[0][2]);
    if (cos(Orientation.y) != 0)
    {
        Orientation.x = atan2(Row[1][2], Row[2][2]);
        Orientation.z = atan2(Row[0][1], Row[0][0]);
    }
    else
    {
        Orientation.x = atan2(-Row[2][0], Row[1][1]);
        Orientation.z = 0;
    }

    //orientation is a quaternion
     //int i, j, k = 0;
     //T root, trace = Row[0].x + Row[1].y + Row[2].z;
     //if (trace > static_cast<T>(0))
     //{
     //    root = sqrt(trace + static_cast<T>(1.0));
     //    Orientation.w = static_cast<T>(0.5) * root;
     //    root = static_cast<T>(0.5) / root;
     //    Orientation.x = root * (Row[1].z - Row[2].y);
     //    Orientation.y = root * (Row[2].x - Row[0].z);
     //    Orientation.z = root * (Row[0].y - Row[1].x);
     //} // End if > 0
     //else
     //{
     //    static int Next[3] = { 1, 2, 0 };
     //    i = 0;
     //    if (Row[1].y > Row[0].x) i = 1;
     //    if (Row[2].z > Row[i][i]) i = 2;
     //    j = Next[i];
     //    k = Next[j];

     //    root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

     //    Orientation[i] = static_cast<T>(0.5) * root;
     //    root = static_cast<T>(0.5) / root;
     //    Orientation[j] = root * (Row[i][j] + Row[j][i]);
     //    Orientation[k] = root * (Row[i][k] + Row[k][i]);
     //    Orientation.w = root * (Row[j][k] - Row[k][j]);
     //} // End if <= 0




    return true;
}


void Invert(Mat4Ptr matrix)
{
    matrix->Invert();
}

Matrix4 Inverted(Mat4Param matrix)
{
    return matrix.Inverted();
}

std::ostream& operator<<(std::ostream& os, const Matrix4& rhs)
{
    return os << "[" << rhs._m[0][0] << " " << rhs._m[0][1] << " " << rhs._m[0][2] << " " << rhs._m[0][3] << "]\n"
        << "[" << rhs._m[1][0] << " " << rhs._m[1][1] << " " << rhs._m[1][2] << " " << rhs._m[1][3] << "]\n"
        << "[" << rhs._m[2][0] << " " << rhs._m[2][1] << " " << rhs._m[2][2] << " " << rhs._m[2][3] << "]\n"
        << "[" << rhs._m[3][0] << " " << rhs._m[3][1] << " " << rhs._m[3][2] << " " << rhs._m[3][3] << "]\n";
}