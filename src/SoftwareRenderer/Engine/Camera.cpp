#include "Camera.h"

Camera::Camera() : position(Vec3f(0, 0, 0)), direction(Vec3f(0, 0, 1)), up(Vec3f(0, 1, 0)), forwardVelocity(Vec3f(0, 0, 0)), rightVelocity(Vec3f(0, 0, 0)), yaw(0.0f), pitch(0.0f)
{
}

Camera::Camera(const Vec3f& position, const Vec3f& target, const Vec3f& up) : position(position), direction(target), up(up)
{
}
