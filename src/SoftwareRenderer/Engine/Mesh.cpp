#include "Mesh.h"

Mesh::Mesh() : pTexture(nullptr)
{
  scale = { 1.0f, 1.0f, 1.0f };
  rotation = { 0.0f, 0.0f, 0.0f };
  translation = { 0.0f, 0.0f, 0.0f };
}

Mesh::~Mesh()
{
  if (pTexture != nullptr)
  {
    pTexture->Unload();
  }

  pTexture = nullptr;
}

Texture* Mesh::GetTexture()
{
  return pTexture.get();
}
