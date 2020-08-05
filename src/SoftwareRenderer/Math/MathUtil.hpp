#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"

template <typename T>
Vector4<T> createVec4FromVec3(const Vector3<T>& vec)
{
  return Vector4<T>(vec.x, vec.y, vec.z, 1);
}

template <typename T>
Vector3<T> createVec3FromVec4(const Vector4<T>& vec)
{
  return Vector3<T>(vec.x, vec.y, vec.z);
}

template <typename T>
Matrix4<T> createTranslationMatrix(T x, T y, T z)
{
  return Matrix4<T>
  {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1
  };
}

template <typename T>
Matrix4<T> createTranslationMatrix(const Vector3<T>& translation)
{
  return createTranslationMatrix(translation.x, translation.y, translation.z);
}

template <typename T>
Matrix4<T> createTranslationMatrix(const Vector4<T>& translation)
{
  return createTranslationMatrix(translation.x, translation.y, translation.z);
}

template <typename T>
Matrix4<T> createRotationMatrixX(T angle)
{
  T c = cos(angle);
  T s = sin(angle);
  return Matrix4<T>
  {
    1, 0, 0, 0,
    0, c, -s, 0,
    0, s, c, 0,
    0, 0, 0, 1
  };
}

template <typename T>
Matrix4<T> createRotationMatrixY(T angle)
{
  T c = cos(angle);
  T s = sin(angle);
  return Matrix4<T>
  {
    c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1
  };
}

template <typename T>
Matrix4<T> createRotationMatrixZ(T angle)
{
  T c = cos(angle);
  T s = sin(angle);
  return Matrix4<T>
  {
    c, -s, 0, 0,
    s, c, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };
}

template <typename T>
Matrix4<T> createScaleMatrix(T x, T y, T z)
{
  return Matrix4<T>
  {
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  };
}

template <typename T>
Matrix4<T> createRotationMatrix(T angleX, T angleY, T angleZ)
{
  return createRotationMatrixZ(angleZ) * createRotationMatrixY(angleY) * createRotationMatrixX(angleX);
}

template <typename T>
Matrix4<T> createRotationMatrix(Vector3<T> rotation)
{
  return createRotationMatrix(rotation.x, rotation.y, rotation.z);
}

template <typename T>
Matrix4<T> createScaleMatrix(const Vector3<T>& scale)
{
  return createScaleMatrix(scale.x, scale.y, scale.z);
}

template <typename T>
Matrix4<T> createScaleMatrix(const Vector4<T>& scale)
{
  return createScaleMatrix(scale.x, scale.y, scale.z);
}

//template <typename T>
//Matrix4<T> createPerspectiveMatrix(T fov, T aspect, T near, T far) {
//  T f = 1.0f / tan(fov / 2.0f);
//  return Matrix4<T>{
//      aspect * f , 0, 0, 0,
//      0,           f, 0, 0,
//      0, 0, far / (far - near), (-far * near) / (far - near),
//      0, 0, 1, 0
//  };
//}

template <typename T>
Matrix4<T> createPerspectiveMatrix(T fov, T aspect, T near, T far)
{
  T f = 1.0f / tan(fov / 2.0f);
  return Matrix4<T>
  {
    f / aspect, 0, 0, 0,
    0, f, 0, 0,
    0, 0, (far + near) / (near - far), (2 * far * near) / (near - far),
    0, 0, -1, 0
  };
}

//template <typename T>
//Matrix4<T> createPerspectiveMatrix(T fov, T aspect, T near, T far) {
//  T f = 1.0f / tan(fov / 2.0f);
//  return Matrix4<T>{
//    aspect* (1.0f / tan(fov / 2.0f)), 0, 0, 0,
//      0, 1.0f / tan(fov / 2.0f), 0, 0,
//      0, 0, far / (far - near), (-far * near) / (far - near),
//      0, 0, -1, 0
//  };
//}


template <typename T>
Vector4 <T> project(Matrix4<T>  mat_proj, Vec4f v)
{
  // multiply the projection matrix by our original vector
  Vec4f result = mat_proj * v;

  // perform perspective divide with original z-value that is now stored in w
  if (result.w != 0.0)
  {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  }
  return result;
}

//template <typename T>
//Vector4 <T> project(const Matrix4<T>& projectionMatrix, const Vector4<T>& vec) {
//  Vector4<T> result = projectionMatrix * vec;
//  if (result.w != 0) {
//    return Vector4<T>{
//      result.x / result.w,
//      result.y / result.w,
//      result.z / result.w,
//      1 };
//  }
//
//  return result;
//}

template <typename T>
Matrix4<T> createOrthographicMatrix(T left, T right, T bottom, T top, T near, T far)
{
  return Matrix4<T>
  {
    2 / (right - left), 0, 0, 0,
    0, 2 / (top - bottom), 0, 0,
    0, 0, -2 / (far - near), 0,
    -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
  };
}

template <typename T>
Matrix4<T> createLookAtMatrix(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
{
  Vector3<T> zAxis = target - eye;
  zAxis.Normalize();
  Vector3<T> xAxis = Cross(up, zAxis);
  xAxis.Normalize();
  Vector3<T> yAxis = Cross(zAxis, xAxis);

  return Matrix4<T>
  {
    xAxis.x, xAxis.y, xAxis.z, -Dot(xAxis, eye),
    yAxis.x, yAxis.y, yAxis.z, -Dot(yAxis, eye),
    zAxis.x, zAxis.y, zAxis.z, -Dot(zAxis, eye),
    0, 0, 0, 1
  };
}
