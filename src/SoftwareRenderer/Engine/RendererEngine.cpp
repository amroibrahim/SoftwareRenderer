#include "RendererEngine.h"

#include <iostream>
#include <algorithm>

#include "Utilities.h"
#include "SDL_Draw.h"
#include "SDL_Draw_Helper.h"
#include "Mesh.h"


using namespace std;

RendererEngine::RendererEngine(SDL_Renderer* pSDLRenderer, Configuration* pConfiguration) : m_pRenderBuffer(nullptr), m_pDepthBuffer(nullptr), m_pSDLRenderer(pSDLRenderer), m_pConfiguration(pConfiguration)
{
  m_iWidth = m_pConfiguration->display.iScreenBufferWidth;
  m_iHight = m_pConfiguration->display.iScreenBufferHeight;
  m_pRenderBuffer = new uint32_t[m_iWidth * m_iHight];
  m_pDepthBuffer = new float[m_iWidth * m_iHight];
}

RendererEngine::~RendererEngine()
{
  if (m_pRenderBuffer)
  {
    delete[] m_pRenderBuffer;
  }

  if (m_pDepthBuffer)
  {
    delete[] m_pDepthBuffer;
  }
}

void RendererEngine::ClearSurface(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  ColorConverter colorConverter{};
  colorConverter.r = r;
  colorConverter.g = g;
  colorConverter.b = b;
  colorConverter.a = a;

  for (uint32_t y = 0; y < m_iHight; ++y)
  {
    for (uint32_t x = 0; x < m_iWidth; ++x)
    {
      SDL_SetPixel(m_pRenderBuffer, m_iWidth, m_iHight, x, y, colorConverter.color);
      m_pDepthBuffer[y * m_iWidth + x] = 1.0f;
    }
  }
}

void RendererEngine::ClearSurface(uint32_t argb)
{
  for (uint32_t i = 0; i < m_iWidth * m_iHight; ++i)
  {
    m_pRenderBuffer[i] = argb;
    m_pDepthBuffer[i] = 1.0f;
  }
}

void RendererEngine::Render(Mesh* pModel)
{
  if (m_pConfiguration->render.renderGrid)
  {
    DrawGrid(m_pConfiguration->render.gridSpacing, m_pConfiguration->render.gridSpacing);
  }

  ColorConverter colorWireframeConverter{};
  colorWireframeConverter.r = m_pConfiguration->render.renderWireframeColorR;
  colorWireframeConverter.g = m_pConfiguration->render.renderWireframeColorG;
  colorWireframeConverter.b = m_pConfiguration->render.renderWireframeColorB;
  colorWireframeConverter.a = 255;

  // Loop all projected triangles and render them
  for (int i = 0; i < pModel->trianglesToRender.size(); i++)
  {
    triangle_t& triangle = pModel->trianglesToRender[i];
    if (m_pConfiguration->render.renderFillTriangles)
    {
      SDL_DrawFilledTriangle(m_pRenderBuffer, m_iWidth, m_iHight, triangle.vertices, triangle.colors);
    }

    if (m_pConfiguration->render.renderTexturedTriangles)
    {
      if (pModel->pTexture != nullptr && pModel->pTexture->GetTexture() != nullptr)
      {
        // Debugging draw texture
        // Debug_DrawTexture(m_pRenderBuffer, m_iWidth, m_iHight, pModel->pTexture->GetTexture(), pModel->pTexture->GetWidth(), pModel->pTexture->GetHeight());

        SDL_DrawTexturedTriangle(m_pRenderBuffer, m_iWidth, m_iHight, m_pDepthBuffer, triangle.vertices, triangle.uvs, pModel->pTexture->GetTexture(), pModel->pTexture->GetWidth(), pModel->pTexture->GetHeight());
      }

    }

    if (m_pConfiguration->render.renderWireframe)
    {
      SDL_DrawTriangle(m_pRenderBuffer, m_iWidth, m_iHight, triangle.vertices, colorWireframeConverter.color);
    }
  }
}

void RendererEngine::RenderTriangle(Vec3f* pPoint, uint32_t* colors, bool bFilled)
{
  if (m_pConfiguration->render.renderGrid)
  {
    DrawGrid(m_pConfiguration->render.gridSpacing, m_pConfiguration->render.gridSpacing);
  }

  ColorConverter colorConverter{};
  colorConverter.r = m_pConfiguration->render.renderDrawColorR;
  colorConverter.g = m_pConfiguration->render.renderDrawColorG;
  colorConverter.b = m_pConfiguration->render.renderDrawColorB;
  colorConverter.a = 255;

  if (bFilled)
  {
    Vec4f points[3] =
    {
      {pPoint[0].x, pPoint[0].y, 0, 0},
      {pPoint[1].x, pPoint[1].y, 0, 0},
      {pPoint[2].x, pPoint[2].y, 0, 0}
    };

    SDL_DrawFilledTriangle(m_pRenderBuffer, m_iWidth, m_iHight, points, colors);
  }
  else
  {
    Vec4f points[3] =
    {
      { pPoint[0].x, pPoint[0].y, 0, 0 },
      { pPoint[1].x, pPoint[1].y, 0, 0 },
      { pPoint[2].x, pPoint[2].y, 0, 0 }
    };
    SDL_DrawTriangle(m_pRenderBuffer, m_iWidth, m_iHight, points, colorConverter.color);
  }
}

void RendererEngine::Render(Vec3f* pPoint, int iCount)
{
  if (m_pConfiguration->render.renderGrid)
  {
    DrawGrid(m_pConfiguration->render.gridSpacing, m_pConfiguration->render.gridSpacing);
  }

  ColorConverter colorConverter{};
  colorConverter.r = m_pConfiguration->render.renderDrawColorR;
  colorConverter.g = m_pConfiguration->render.renderDrawColorG;
  colorConverter.b = m_pConfiguration->render.renderDrawColorB;
  colorConverter.a = 255;

  for (int i = 0; i < iCount; ++i)
  {
    SDL_SetPixel(m_pRenderBuffer, m_iWidth, m_iHight, static_cast<uint32_t>(pPoint[i].x), static_cast<uint32_t>(pPoint[i].y), colorConverter.color);
  }
}

Vec3f RendererEngine::ProjectPoint(Vec3f fPoint)
{
  float fovFactor = m_pConfiguration->render.fovFactor;
  return fPoint * fovFactor / fPoint.z;
}

Vec4f RendererEngine::ProjectPoint(Vec4f fPoint)
{
  float fovFactor = m_pConfiguration->render.fovFactor;
  return fPoint * fovFactor / fPoint.z;
}

uint32_t* RendererEngine::GetRenderBuffer()
{
  return m_pRenderBuffer;
}

//void RendererEngine::DrawMesh(Mesh* pModel)
//{
//  for (size_t i = 0; i < pModel->Faces.size(); ++i)
//  {
//    DrawLine(MapX(pModel->Vertices[pModel->Faces[i][0]].x), MapY(pModel->Vertices[pModel->Faces[i][0]].y), MapX(pModel->Vertices[pModel->Faces[i][1]].x), MapY(pModel->Vertices[pModel->Faces[i][1]].y));
//    DrawLine(MapX(pModel->Vertices[pModel->Faces[i][1]].x), MapY(pModel->Vertices[pModel->Faces[i][1]].y), MapX(pModel->Vertices[pModel->Faces[i][2]].x), MapY(pModel->Vertices[pModel->Faces[i][2]].y));
//    DrawLine(MapX(pModel->Vertices[pModel->Faces[i][2]].x), MapY(pModel->Vertices[pModel->Faces[i][2]].y), MapX(pModel->Vertices[pModel->Faces[i][0]].x), MapY(pModel->Vertices[pModel->Faces[i][0]].y));
//  }
//}

void RendererEngine::DrawGrid(uint32_t iXSize, uint32_t iYSize)
{
  ColorConverter colorConverter{};

  colorConverter.r = m_pConfiguration->render.gridColorR;
  colorConverter.g = m_pConfiguration->render.gridColorG;
  colorConverter.b = m_pConfiguration->render.gridColorB;
  colorConverter.a = 255;

  for (uint32_t y = 0; y < m_iHight; y += iYSize)
  {
    SDL_DrawLine(m_pRenderBuffer, m_iWidth, m_iHight, 0, y, m_iWidth - 1, y, colorConverter.color);
  }

  for (uint32_t x = 0; x < m_iWidth; x += iXSize)
  {
    SDL_DrawLine(m_pRenderBuffer, m_iWidth, m_iHight, x, 0, x, m_iHight - 1, colorConverter.color);
  }
}

Uint8 RendererEngine::MapColor(float color)
{
  return static_cast<Uint8>(color * 255.0f);
}

int RendererEngine::MapX(float x)
{
  return static_cast<int>((x + 1) * m_iWidth / 2);
}

int RendererEngine::MapY(float y)
{
  return static_cast<int>((y + 1) * (m_iHight / 2 - 1));
}
