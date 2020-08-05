#pragma once
#include <cstdint>
#include "Vector4.hpp"

template <typename T>
class Matrix4
{
public:
  static const Matrix4<T> Identity;

  T m[4][4] =
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };

  Matrix4() = default;
  Matrix4(T m00, T m01, T m02, T m03,
          T m10, T m11, T m12, T m13,
          T m20, T m21, T m22, T m23,
          T m30, T m31, T m32, T m33)
  {
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
  }

  Matrix4(const Vector4<T>& a, const Vector4<T>& b, const Vector4<T>& c, const Vector4<T>& d)
  {
    m[0][0] = a.x;
    m[0][1] = a.y;
    m[0][2] = a.z;
    m[0][3] = a.w;
    m[1][0] = b.x;
    m[1][1] = b.y;
    m[1][2] = b.z;
    m[1][3] = b.w;
    m[2][0] = c.x;
    m[2][1] = c.y;
    m[2][2] = c.z;
    m[2][3] = c.w;
    m[3][0] = d.x;
    m[3][1] = d.y;
    m[3][2] = d.z;
    m[3][3] = d.w;
  }

  const Matrix4<T> operator * (const Matrix4<T>& other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.m[i][j] =
          m[i][0] * other.m[0][j] +
          m[i][1] * other.m[1][j] +
          m[i][2] * other.m[2][j] +
          m[i][3] * other.m[3][j];
      }
    }
    return result;
  }

  const Vector4<T> operator * (const Vector4<T>& v) const
  {
    Vector4<T> result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
    return result;
  }

  const Matrix4<T> operator + (const Matrix4<T>& other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.m[i][j] = m[i][j] + other.m[i][j];
      }
    }
    return result;
  }

  const Matrix4<T> operator - (const Matrix4<T>& other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.m[i][j] = m[i][j] - other.m[i][j];
      }
    }
    return result;
  }

  const T* operator [] (uint8_t i) const
  {
    return m[i];
  }

  T* operator [] (uint8_t i)
  {
    return m[i];
  }
};

typedef Matrix4<float> Mat4f;
