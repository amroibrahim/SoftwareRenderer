// SDL Draw

#ifndef SDL_DRAW_HELPER_H
#define SDL_DRAW_HELPER_H

#include <SDL.h>
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include "SDL_Draw.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Draw Triangle to a buffer
 */
void SDL_DrawTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t color);

enum SDL_DRAW_FILLED_TRIANGLE_ALGORITHM
{
  BARYCENTRIC_BOUNDING_BOX,
  FLAT_TOP_FLAT_BOTTOM
};

void SDL_DrawFilledTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color);

void SDL_DrawFilledTriangle_BarycentricBoundingBox(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color);
void SDL_DrawFilledTriangle_FlatTopFlatBottom(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color);

enum SDL_DRAW_TEXTURED_TRIANGLE_ALGORITHM
{
  TEXTURED_BARYCENTRIC_FLAT_TOP_FLAT_BOTTOM
};

void SDL_DrawTexturedTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, float* pDepthBuffer, Vec4f* triPoints, Vec2f* triUVs, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight);

void SDL_DrawTextured_Barycentric_FlatTopFlatBottom(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, float* pDepthBuffer, Vec4f* triPoints, Vec2f* triUVs, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight);

void CalculateBarycentricCoordinate(Vec4f* triPoints,  Vec4f* point, Vec3f* outCoordinate);

void Debug_DrawTexture(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight);

#ifdef __cplusplus
} //extern "C"
#endif

#endif  // SDL_DRAW_H
