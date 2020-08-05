#pragma once
#include <cmath>

template <typename T>

class Vector3
{
public:
  T x, y, z;

  Vector3() : x(0), y(0), z(0)
  {
  }

  Vector3(const T& value) : x(value), y(value), z(value)
  {
  }

  Vector3(T x, T y, T z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  T& operator[](int i)
  {
    return ((&x)[i]);
  }

  const T& operator[](int i) const
  {
    return ((&x)[i]);
  }

  Vector3<T>& operator *= (float s)
  {
    x *= s;
    y *= s;
    z *= s;
    return (*this);
  }

  Vector3<T>& operator /=(float s)
  {
    s = 1.0F / s;
    x *= s;
    y *= s;
    z *= s;
    return (*this);
  }

  Vector3<T>& operator +=(const Vector3<T>& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
  }

  Vector3<T>& operator -=(const Vector3<T>& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return (*this);
  }

  T Magnitude(const Vector3<T>& v)
  {

    return ((T)sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
  }

  /*
  Vector3<T> Normalize(const Vector3<T> &v) {
    return (v / Magnitude(v));
  }
  */

  Vector3<T>& Normalize()
  {
    T length = Magnitude(*this);
    if (length > 0)
    {
      T invertlength = 1 / length;

      x *= invertlength;
      y *= invertlength;
      z *= invertlength;
    }
    return (*this);
  }

  T Dot(const Vector3<T>& v) const
  {
    return (x * v.x + y * v.y + z * v.z);
  }

  Vector3<T> Cross(const Vector3<T>& v) const
  {
    return Vector3<T>(
             y * v.z - z * v.y,
             z * v.x - x * v.z,
             x * v.y - y * v.x);
  }
};

template<class T>
T Dot(const Vector3<T>& lhsv, const Vector3<T>& rhsv)
{
  return (lhsv.x * rhsv.x + lhsv.y * rhsv.y + lhsv.z * rhsv.z);
}


template<class T>
Vector3<T> Cross(const Vector3<T>& lhsv, const Vector3<T>& rhsv)
{
  return Vector3<T>(
           lhsv.y * rhsv.z - lhsv.z * rhsv.y,
           lhsv.z * rhsv.x - lhsv.x * rhsv.z,
           lhsv.x * rhsv.y - lhsv.y * rhsv.x);
}

template<class T>
inline Vector3<T> operator *(const Vector3<T>& v, float s)
{
  return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template<class T>
inline Vector3<T> operator /(const Vector3<T>& v, float s)
{
  s = 1.0F / s;
  return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template<class T>
inline Vector3<T> operator +(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
  return (Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z));
}

template<class T>
inline Vector3<T> operator -(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
  return (Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z));
}

template<class T>
inline Vector3<T> RotateX(const Vector3<T>& v, float angle)
{
  Vector3<T> rotated_vector =
  {
    v.x,
    v.y * (float)cos(angle) - v.z * (float)sin(angle),
    v.y * (float)sin(angle) + v.z * (float)cos(angle)
  };
  return rotated_vector;
}

template<class T>
inline Vector3<T> RotateY(const Vector3<T>& v, float angle)
{
  Vector3<T> rotated_vector =
  {
    v.x * (float)cos(angle) - v.z * (float)sin(angle),
    v.y,
    v.x * (float)sin(angle) + v.z * (float)cos(angle)
  };
  return rotated_vector;
}

template<class T>
inline Vector3<T> RotateZ(const Vector3<T>& v, float angle)
{
  Vector3<T> rotated_vector =
  {
    v.x * (float)cos(angle) - v.y * (float)sin(angle),
    v.x * (float)sin(angle) + v.y * (float)cos(angle),
    v.z
  };
  return rotated_vector;
}

typedef Vector3<float> Vec3f;
typedef Vector3<float> Point3f;
typedef Vector3<double> Vec3d;
typedef Vector3<double> Point3d;
