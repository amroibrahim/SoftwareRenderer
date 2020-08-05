#pragma once

#include <SDL.h>

#include <vector>

#include "Configuration.h"
#include "Mesh.h"
#include "Vector3.hpp"
#include "Vector4.hpp"

class RendererEngine
{
public:
  RendererEngine(SDL_Renderer* pSDLRenderer, Configuration* pConfiguration);
  ~RendererEngine();

  void ClearSurface(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
  void ClearSurface(uint32_t argb);
  /*void DrawMesh(Mesh* pModel);*/
  void Render(Mesh* pModel);
  void Render(Vec3f* pPoint, int iCount);
  void RenderTriangle(Vec3f* pPoint, uint32_t* colors, bool bFilled);

  void DrawGrid(uint32_t iXSize, uint32_t iYSize);
  Vec3f ProjectPoint(Vec3f fPoint);
  Vec4f ProjectPoint(Vec4f fPoint);

  uint32_t* GetRenderBuffer();

protected:
  Uint8 MapColor(float color);
  int MapX(float x);
  int MapY(float y);

  // Render Buffer is 32bits ARGB format
  uint32_t* m_pRenderBuffer;
  float* m_pDepthBuffer;

  SDL_Renderer* m_pSDLRenderer;

  Configuration* m_pConfiguration;

  uint32_t m_iWidth;
  uint32_t m_iHight;
};

