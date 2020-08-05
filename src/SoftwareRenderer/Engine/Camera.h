#pragma once
#include "Vector3.hpp"

class Camera
{
public:
  Vec3f position;
  Vec3f direction;
  Vec3f up;

  Vec3f forwardVelocity;
  Vec3f rightVelocity;

  float yaw;
  float pitch;

  Camera();
  Camera(const Vec3f& position, const Vec3f& target, const Vec3f& up);
};

