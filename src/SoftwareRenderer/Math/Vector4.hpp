#pragma once

template <typename T>

class Vector4
{
public:
  T x, y, z, w;

  Vector4() : x(0), y(0), z(0), w(0)
  {
  }

  Vector4(const T& value) : x(value), y(value), z(value), w(value)
  {
  }

  Vector4(T x, T y, T z, T w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  T& operator[](int i)
  {
    return ((&x)[i]);
  }

  const T& operator[](int i) const
  {
    return ((&x)[i]);
  }

  Vector4<T>& operator *= (float s)
  {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return (*this);
  }

  Vector4<T>& operator /=(float s)
  {
    s = 1.0F / s;
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return (*this);
  }

  Vector4<T>& operator +=(const Vector4<T>& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return (*this);
  }

  Vector4<T>& operator -=(const Vector4<T>& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return (*this);
  }

  T Magnitude(const Vector4<T>& v)
  {
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
  }

  /*
  Vector3<T> Normalize(const Vector3<T> &v) {
    return (v / Magnitude(v));
  }
  */

  Vector4<T>& Normalize()
  {
    T length = Magnitude();
    if (length > 0)
    {
      T invertlength = 1 / length;

      x *= invertlength;
      y *= invertlength;
      z *= invertlength;
      w *= invertlength;
    }
    return (*this);
  }

  T Dot(const Vector4<T>& v) const
  {
    return (x * v.x + y * v.y + z * v.z);
  }

  Vector4<T> Cross(const Vector4<T>& v) const
  {
    return Vector4<T>(
             y * v.z - z * v.y,
             z * v.x - x * v.z,
             x * v.y - y * v.x);
  }
};

template<class T>
T Dot(const Vector4<T>& lhsv, const Vector4<T>& rhsv)
{
  return (lhsv.x * rhsv.x + lhsv.y * rhsv.y + lhsv.z * rhsv.z);
}

template<class T>
T Dot(const Vector4<T>& lhsv, const Vector3<T>& rhsv)
{
  return (lhsv.x * rhsv.x + lhsv.y * rhsv.y + lhsv.z * rhsv.z);
}

template<class T>
Vector4<T> Cross(const Vector4<T>& lhsv, const Vector4<T>& rhsv)
{
  return Vector4<T>(
           lhsv.y * rhsv.z - lhsv.z * rhsv.y,
           lhsv.z * rhsv.x - lhsv.x * rhsv.z,
           lhsv.x * rhsv.y - lhsv.y * rhsv.x);
}

template<class T>
inline Vector4<T> operator *(const Vector4<T>& v, float s)
{
  return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template<class T>
inline Vector4<T> operator /(const Vector4<T>& v, float s)
{
  s = 1.0F / s;
  return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template<class T>
inline Vector4<T> operator +(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
  return (Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w));
}

template<class T>
inline Vector4<T> operator -(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
  return (Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w));
}

template<class T>
inline Vector4<T> operator -(const Vector4<T>& lhs, const Vector3<T>& rhs)
{
  return (Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w));
}

template<class T>
inline Vector4<T> RotateX(const Vector4<T>& v, T angle)
{
  Vector4<T> rotated_vector =
  {
    v.x,
    v.y * cos(angle) - v.z * sin(angle),
    v.y * sin(angle) + v.z * cos(angle),
    v.w
  };
  return rotated_vector;
}

template<class T>
inline Vector4<T> RotateY(const Vector4<T>& v, T angle)
{
  Vector4<T> rotated_vector =
  {
    v.x * cos(angle) - v.z * sin(angle),
    v.y,
    v.x * sin(angle) + v.z * cos(angle),
    v.w
  };
  return rotated_vector;
}

template<class T>
inline Vector4<T> RotateZ(const Vector4<T>& v, T angle)
{
  Vector4<T> rotated_vector =
  {
    v.x* cos(angle) - v.y * sin(angle),
    v.x* sin(angle) + v.y * cos(angle),
    v.z,
    v.w
  };
  return rotated_vector;
}

typedef Vector4<float> Vec4f;
typedef Vector4<float> Point4f;
typedef Vector4<double> Vec4d;
typedef Vector4<double> Point4d;
