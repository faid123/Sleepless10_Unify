#include "Precompiled.h"
#include "Matrix3.h"


const Matrix3 Matrix3::cIdentity(1.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f);


Matrix3::Matrix3(float p00, float p01, float p02, 
                 float p10, float p11, float p12, 
                 float p20, float p21, float p22)
{
    m00 = p00;  m01 = p01;  m02 = p02;
    m10 = p10;  m11 = p11;  m12 = p12; 
    m20 = p20;  m21 = p21;  m22 = p22;
}

///////////////// Matrix 3x3 ///////////////////////////////
Matrix3::Matrix3(const glm::mat3& rhs)
{
    m00 = rhs[0][0];
    m01 = rhs[0][1];
    m02 = rhs[0][2];
    m10 = rhs[1][0];
    m11 = rhs[1][1];
    m12 = rhs[1][2];
    m20 = rhs[2][0];
    m21 = rhs[2][1];
    m22 = rhs[2][2];
}
Matrix3::Matrix3(const physx::PxMat33& rhs)
{
    m00 = rhs[0][0];
    m01 = rhs[0][1];
    m02 = rhs[0][2];
    m10 = rhs[1][0];
    m11 = rhs[1][1];
    m12 = rhs[1][2];
    m20 = rhs[2][0];
    m21 = rhs[2][1];
    m22 = rhs[2][2];
}
glm::mat3 Matrix3::Mtx3toGlmMat3()
{
    glm::mat3 tmp;
    tmp[0][0] = m00;
    tmp[0][1] = m01;
    tmp[0][2] = m02;
    tmp[1][0] = m10;
    tmp[1][1] = m11;
    tmp[1][2] = m12;
    tmp[2][0] = m20;
    tmp[2][1] = m21;
    tmp[2][2] = m22;

   // glm::mat3 tmp2 = glm::transpose(tmp);
    return tmp;
}
physx::PxMat33 Matrix3::Mtx3toPxMat3()
{
    physx::PxMat33 tmp;
    tmp[0][0] = m00;
    tmp[0][1] = m01;
    tmp[0][2] = m02;
    tmp[1][0] = m10;
    tmp[1][1] = m11;
    tmp[1][2] = m12;
    tmp[2][0] = m20;
    tmp[2][1] = m21;
    tmp[2][2] = m22;

   // physx::PxMat33 tmp2 = tmp.getTranspose();
    return tmp;
}
//------------------------------------------ Binary Assignment Operators 

void Matrix3::operator*=(float rhs)
{
    vect[0] *= rhs;
    vect[1] *= rhs;
    vect[2] *= rhs;
}

void Matrix3::operator/=(float rhs)
{
    if(rhs)
    {
        vect[0] /= rhs;
        vect[1] /= rhs;
        vect[2] /= rhs; 
    }
}

//----------------------------------------------------- Binary Operators

Matrix3 Matrix3::operator*(float rhs) const
{
    Matrix3 ret = *this;
    ret *= rhs;
    return ret;
}

Matrix3 Matrix3::operator/(float rhs) const
{
    Matrix3 ret;
    if(rhs)
    {
        ret = *this;
        ret /= rhs;
    }
    return ret;
}

//---------------------------------------- Binary Assignment Operator 

void Matrix3::operator+=(Mat3Param rhs)
{
  vect[0] += rhs.vect[0];
  vect[1] += rhs.vect[1];
  vect[2] += rhs.vect[2];
}

void Matrix3::operator-=(Mat3Param rhs)
{
  vect[0] -= rhs.vect[0];
  vect[1] -= rhs.vect[1];
  vect[2] -= rhs.vect[2];
}

//-------------------------------------------------- Binary Operators 
Matrix3 Matrix3::operator+(Mat3Param rhs) const
{
  Matrix3 ret = *this;
  ret += rhs;
  return ret;
}

Matrix3 Matrix3::operator-(Mat3Param rhs) const
{
  Matrix3 ret = *this;
  ret -= rhs;
  return ret;
}

Matrix3 Matrix3::operator*(Mat3Param rhs) const
{
  return Concat(rhs);
}

//----------------------------------------------------------- Binary Comparisons

bool Matrix3::operator==(Mat3Param rhs) const
{
  return vect[0] == rhs.vect[0] &&
         vect[1] == rhs.vect[1] &&
         vect[2] == rhs.vect[2];
}

bool Matrix3::operator!=(Mat3Param rhs) const
{
  return !(*this == rhs);
}

float Matrix3::operator()(unsigned r, unsigned c) const
{
    if (r <= 2 && c <= 2)
    {
        return array[r + c * 3];
    }
    else
        return 0.0f;
}

//float& Matrix3::operator()(unsigned r, unsigned c)
//{
//    float tmp=0.0f;
//    if (r <= 2 && c <= 2)
//    {
//        tmp = array[r + c * 3];
//    }
//    return tmp;
//}
Vector3& Matrix3::operator[](unsigned index)
{
    return ((Vector3*)this)[index];
}

const Vector3& Matrix3::operator[](unsigned index) const
{ 
    return ((Vector3*)this)[index];
}


Matrix3 Matrix3::Transposed() const
{
  Matrix3 ret;

  ret.m00 = m00;
  ret.m01 = m10;
  ret.m02 = m20;
  ret.m10 = m01;
  ret.m11 = m11;
  ret.m12 = m21;
  ret.m20 = m02;
  ret.m21 = m12;
  ret.m22 = m22;

  return ret;
}

Mat3Ref Matrix3::Transpose()
{
    std::swap(m01, m10);
    std::swap(m02, m20);
    std::swap(m12, m21);
    return *this;
}

Matrix3 Matrix3::Inverted() const
{
    Matrix3 foo(*this);
    foo.Invert();
    return foo;
}


Matrix3 Matrix3::Invert()
{
    Matrix3 tmp;
    float determinant = Determinant();
    if(determinant != 0.0f)
    {
        determinant = 1.0f / determinant;
        float t00 = (m11 * m22 - m12 * m21) * determinant;
        float t01 = (m02 * m21 - m01 * m22) * determinant;
        float t02 = (m01 * m12 - m02 * m11) * determinant;
        float t10 = (m12 * m20 - m10 * m22) * determinant;
        float t11 = (m00 * m22 - m02 * m20) * determinant;
        float t12 = (m02 * m10 - m00 * m12) * determinant;
        float t20 = (m10 * m21 - m11 * m20) * determinant;
        float t21 = (m01 * m20 - m00 * m21) * determinant;
        float t22 = (m00 * m11 - m01 * m10) * determinant;

        m00 = t00;  m01 = t01;  m02 = t02;
        m10 = t10;  m11 = t11;  m12 = t12;
        m20 = t20;  m21 = t21;  m22 = t22;
        tmp = *this;
    }
     return tmp;
}


Matrix3 Matrix3::Concat(Mat3Param rhs) const
{
    Matrix3 ret;

    ret.m00 = Dot(Cross(0), rhs.Basis(0));
    ret.m01 = Dot(Cross(0), rhs.Basis(1));
    ret.m02 = Dot(Cross(0), rhs.Basis(2));

    ret.m10 = Dot(Cross(1), rhs.Basis(0));
    ret.m11 = Dot(Cross(1), rhs.Basis(1));
    ret.m12 = Dot(Cross(1), rhs.Basis(2));

    ret.m20 = Dot(Cross(2), rhs.Basis(0));
    ret.m21 = Dot(Cross(2), rhs.Basis(1));
    ret.m22 = Dot(Cross(2), rhs.Basis(2));

    return ret;
}

Mat3Ref Matrix3::SetIdentity()
{
    vect[0].Set(1.0f, 0.0f, 0.0f);
    vect[1].Set(0.0f, 1.0f, 0.0f);
    vect[2].Set(0.0f, 0.0f, 1.0f);
    return *this;
}

Mat3Ref Matrix3::ZeroOut()
{
    vect[0].ZeroOut();
    vect[1].ZeroOut();
    vect[2].ZeroOut();
    return *this;
}


float Matrix3::Determinant() const
{
    return (m00 * m11 * m22 + m10 * m21 * m02 + m01 * m12 * m20) 
        - (m02 * m11 * m20 + m10 * m01 * m22 + m00 * m21 * m12);
}

Vector3 Matrix3::Basis(unsigned index) const
{
    Vector3 tmp;
    if(index <= 2)
        tmp = vect[index];
    return tmp;
}

Vector3 Matrix3::BasisX() const
{
  return vect[0];
}

Vector3 Matrix3::BasisY() const
{
  return vect[1];
}

Vector3 Matrix3::BasisZ() const
{
  return vect[2];
}

Vector3 Matrix3::Cross(unsigned index) const
{
    Vector3 tmp;
    if (index <= 2)
        return Vector3(array[index], array[3 + index], array[6 + index]);
    else
        return tmp;
}


void Matrix3::Scale(float x, float y, float z)
{
    SetIdentity();
    m00 = x;
    m11 = y;
    m22 = z;
}



void Matrix3::Scale(Vec3Param rhs)
{
    Scale(rhs[0], rhs[1], rhs[2]);
}

void Matrix3::Rotate(float x, float y, float z, float degrees)
{
    float radian = static_cast<float>(degrees * (M_PI/180.0f));

    float c0 = std::cos(radian);
    float n1C0 = 1.0f - c0;
    float s0 = std::sin(radian);

    m00 = x * x * n1C0 + c0;
    m01 = x * y * n1C0 - z * s0;
    m02 = x * z * n1C0 + y * s0;

    m10 = x * y * n1C0 + z * s0;
    m11 = y * y * n1C0 + c0;
    m12 = y * z * n1C0 - x * s0;

    m20 = x * z * n1C0 - y * s0;
    m21 = y * z * n1C0 + x * s0;
    m22 = z * z * n1C0 + c0;
}

void Matrix3::Rotate(Vec3Param rhs, float degrees)
{
    Rotate(rhs.x, rhs.y, rhs.z, degrees);
}

void Matrix3::Translate(float x, float y)
{
    m00 = 1.0f;  m01 = 0.0f;  m02 = x;
    m10 = 0.0f;  m11 = 1.0f;  m12 = y;
    m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f;
}

void Matrix3::Translate(Vec2Param rhs)
{
    Translate(rhs.x, rhs.y);
}


Matrix3 operator*(float lhs, Mat3Param rhs)
{
  return rhs * lhs;
}

Matrix3 Concat(Mat3Param lhs, Mat3Param rhs)
{
  return lhs.Concat(rhs);
}

void Invert(Mat3Ptr matrix)
{
  matrix->Invert();
}

Matrix3 Inverted(Mat3Param matrix)
{
  return matrix.Inverted();
}