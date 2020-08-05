#pragma once

template <typename T>

class Vector2
{
public:
  T x, y;

  Vector2() : x(0), y(0)
  {
  }

  Vector2(const T& value) : x(value), y(value)
  {
  }

  Vector2(T x, T y)
  {
    this->x = x;
    this->y = y;
  }

  T& operator[](int i)
  {
    return ((&x)[i]);
  }

  const T& operator[](int i) const
  {
    return ((&x)[i]);
  }

  Vector2<T>& operator *= (float s)
  {
    x *= s;
    y *= s;
    return (*this);
  }

  Vector2<T>& operator /=(float s)
  {
    s = 1.0F / s;
    x *= s;
    y *= s;
    return (*this);
  }

  Vector2<T>& operator +=(const Vector2<T>& v)
  {
    x += v.x;
    y += v.y;
    return (*this);
  }

  Vector2<T>& operator -=(const Vector2<T>& v)
  {
    x -= v.x;
    y -= v.y;
    return (*this);
  }

  T Magnitude(const Vector2<T>& v)
  {
    return (sqrt(v.x * v.x + v.y * v.y));
  }

  /*
  Vector2<T> Normalize(const Vector2<T> &v) {
    return (v / Magnitude(v));
  }
  */

  Vector2<T>& Normalize()
  {
    T length = Magnitude();
    if (length > 0)
    {
      T invertlength = 1 / length;

      x *= invertlength;
      y *= invertlength;
    }
    return (*this);
  }
};

template<class T>
inline Vector2<T> operator *(const Vector2<T>& v, float s)
{
  return Vector2<T>(v.x * s, v.y * s);
}

template<class T>
inline Vector2<T> operator /(const Vector2<T>& v, float s)
{
  s = 1.0F / s;
  return Vector2<T>(v.x * s, v.y * s);
}

template<class T>
inline Vector2<T> operator +(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
  return (Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y));
}

template<class T>
inline Vector2<T> operator -(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
  return (Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y));
}

template<class T>
inline Vector2<T> operator -(const T& lhs, const Vector2<T>& rhs)
{
  return (Vector2<T>(lhs - rhs.x, lhs - rhs.y));
}

typedef Vector2<int> Vec2i;
typedef Vector2<float> Vec2f;
typedef Vector2<float> Point2f;
typedef Vector2<double> Vec2d;
typedef Vector2<double> Point2d;
