#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Texture.h"
#include "MeshPrimitives.h"

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"


class Mesh
{
public:
  Mesh();
  ~Mesh();

  Texture* GetTexture();

  std::vector<Vec3f> vertices;
  std::vector<Vec3f> normals;
  std::vector<Vec2f> uvs;
  std::vector<face_t> faces;

  std::vector<triangle_t> trianglesToRender;

  Vec3f rotation;
  Vec3f scale;
  Vec3f translation;
  std::unique_ptr<Texture> pTexture;

  float TranslationZ;
  std::string objFileName;
  std::string textureFileName;
};

