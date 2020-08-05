#pragma once
#include <vector>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

struct face_t
{
  std::vector<int> vertices;
  std::vector<int> uvs;
  std::vector<int> normals;
};

struct triangle_t
{
  Vec4f vertices[3];
  Vec2f uvs[3];
  uint32_t colors[3];
  float depth;
};

struct plane_t
{
  Vec3f vertex;
  Vec3f normal;
};

struct polygon_t
{
  constexpr static int MAX_VERTICES = 8;
  uint8_t numVertices = 0;

  Vec2f uvs[MAX_VERTICES];
  Vec3f normals[MAX_VERTICES];
  Vec4f vertices[MAX_VERTICES];
};

enum frustum_plane_t
{
  LEFT_FRUSTUM_PLANE,
  RIGHT_FRUSTUM_PLANE,
  TOP_FRUSTUM_PLANE,
  BOTTOM_FRUSTUM_PLANE,
  NEAR_FRUSTUM_PLANE,
  FAR_FRUSTUM_PLANE,
  NUM_FRUSTUM_PLANES
};
