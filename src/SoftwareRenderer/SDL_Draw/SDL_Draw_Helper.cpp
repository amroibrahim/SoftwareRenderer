#include "SDL_Draw_Helper.h"
#include "Utilities.h"
#include "float.h"
#include <cmath>
#include <algorithm>

#define ROUND(n) ((int) (n + 0.5))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

enum SDL_DRAW_FILLED_TRIANGLE_ALGORITHM sdlDrawTriangleAlgo = FLAT_TOP_FLAT_BOTTOM;
enum SDL_DRAW_TEXTURED_TRIANGLE_ALGORITHM sdlDrawTexturedTriangleAlgo = TEXTURED_BARYCENTRIC_FLAT_TOP_FLAT_BOTTOM;

void SDL_DrawTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* point, uint32_t color)
{
  SDL_DrawLine(pRenderBuffer, iBufferWidth, iBufferHight, static_cast<uint32_t>(point[0].x), static_cast<uint32_t>(point[0].y), static_cast<uint32_t>(point[1].x), static_cast<uint32_t>(point[1].y), color);
  SDL_DrawLine(pRenderBuffer, iBufferWidth, iBufferHight, static_cast<uint32_t>(point[1].x), static_cast<uint32_t>(point[1].y), static_cast<uint32_t>(point[2].x), static_cast<uint32_t>(point[2].y), color);
  SDL_DrawLine(pRenderBuffer, iBufferWidth, iBufferHight, static_cast<uint32_t>(point[2].x), static_cast<uint32_t>(point[2].y), static_cast<uint32_t>(point[0].x), static_cast<uint32_t>(point[0].y), color);
}

void SDL_DrawFilledTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color)
{
  switch (sdlDrawTriangleAlgo)
  {
  case BARYCENTRIC_BOUNDING_BOX:
    SDL_DrawFilledTriangle_BarycentricBoundingBox(pRenderBuffer, iBufferWidth, iBufferHight, triPoints, color);
    break;
  case FLAT_TOP_FLAT_BOTTOM:
    SDL_DrawFilledTriangle_FlatTopFlatBottom(pRenderBuffer, iBufferWidth, iBufferHight, triPoints, color);
    break;
  }
}

void SDL_DrawTexturedTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, float* pDepthBuffer, Vec4f* triPoints, Vec2f* triUVs, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight)
{
  switch (sdlDrawTexturedTriangleAlgo)
  {
  case TEXTURED_BARYCENTRIC_FLAT_TOP_FLAT_BOTTOM:
    SDL_DrawTextured_Barycentric_FlatTopFlatBottom(pRenderBuffer, iBufferWidth, iBufferHight, pDepthBuffer, triPoints, triUVs, texture, iTextureWiedth, iTextureHight);
    break;
  }
}

void SDL_DrawFilledTriangle_BarycentricBoundingBox(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color)
{
  // Find bounding box
  uint32_t xMin = static_cast<uint32_t>MIN(MIN(triPoints[0].x, triPoints[1].x), triPoints[2].x);
  uint32_t xMax = static_cast<uint32_t>MAX(MAX(triPoints[0].x, triPoints[1].x), triPoints[2].x);
  uint32_t yMin = static_cast<uint32_t>MIN(MIN(triPoints[0].y, triPoints[1].y), triPoints[2].y);
  uint32_t yMax = static_cast<uint32_t>MAX(MAX(triPoints[0].y, triPoints[1].y), triPoints[2].y);

  // Clip bounding box
  xMin = MAX(0, xMin);
  xMax = MIN(iBufferWidth - 1, xMax);
  yMin = MAX(0, yMin);
  yMax = MIN(iBufferHight - 1, yMax);

  // Find triangle area
  float area = 0.5f * ((triPoints[1].x - triPoints[0].x) * (triPoints[2].y - triPoints[0].y) - (triPoints[2].x - triPoints[0].x) * (triPoints[1].y - triPoints[0].y));

  // Loop through bounding box
  for (uint32_t y = yMin; y <= yMax; y++)
  {
    for (uint32_t x = xMin; x <= xMax; x++)
    {
      // Calculate barycentric coordinates
      float alpha = 0.5f * ((triPoints[1].x * triPoints[2].y - triPoints[2].x * triPoints[1].y) - (triPoints[1].x * y - x * triPoints[1].y) + (triPoints[2].x * y - x * triPoints[2].y)) / area;
      float beta = 0.5f * ((triPoints[2].x * triPoints[0].y - triPoints[0].x * triPoints[2].y) - (triPoints[2].x * y - x * triPoints[2].y) + (triPoints[0].x * y - x * triPoints[0].y)) / area;
      float gamma = 0.5f * ((triPoints[0].x * triPoints[1].y - triPoints[1].x * triPoints[0].y) - (triPoints[0].x * y - x * triPoints[0].y) + (triPoints[1].x * y - x * triPoints[1].y)) / area;

      // Check if points is inside triangle
      if (alpha >= 0 && beta >= 0 && gamma >= 0)
      {
        // interpolate color
        ColorConverter colorConverter[3] = { color[0], color[1], color[2] };
        ColorConverter colorInterpolated = { 0 };
        colorInterpolated.r = static_cast<uint8_t>(alpha * colorConverter[0].r + beta * colorConverter[1].r + gamma * colorConverter[2].r);
        colorInterpolated.g = static_cast<uint8_t>(alpha * colorConverter[0].g + beta * colorConverter[1].g + gamma * colorConverter[2].g);
        colorInterpolated.b = static_cast<uint8_t>(alpha * colorConverter[0].b + beta * colorConverter[1].b + gamma * colorConverter[2].b);
        colorInterpolated.a = 255;

        SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, colorInterpolated.color);
      }
    }
  }
}

template <typename T>
void swap(T& a, T& b)
{
  T temp = a;
  a = b;
  b = temp;
}

template <typename T>
void clamp(T* x, T clampStart, T clampEnd)
{
  if (*x < clampStart)
  {
    *x = clampStart;
  }
  else if (*x >= clampEnd)
  {
    *x = clampEnd - 1;
  }
}

void SDL_DrawFlatTopTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color)
{
  Vec2i ipoint[3] =
  {
    { static_cast<int>(triPoints[0].x), static_cast<int>(triPoints[0].y) },
    { static_cast<int>(triPoints[1].x), static_cast<int>(triPoints[1].y) },
    { static_cast<int>(triPoints[2].x), static_cast<int>(triPoints[2].y) }
  };

  //  P0 ---- P1
  //     \   |
  //      \  |
  //       \ |
  //        p2

  //float invsLope1 = (ipoint[2].x - ipoint[0].x) / (float)(ipoint[2].y - ipoint[0].y);
  //float invsLope2 = (ipoint[2].x - ipoint[1].x) / (float)(ipoint[2].y - ipoint[1].y);

  float invsLope1 = 0.0f;
  float invsLope2 = 0.0f;

  // Calculate the difference in y-coordinates for slope 1
  float yDiff1 = (float)ipoint[2].y - (float)ipoint[0].y;
  if (abs(yDiff1) > 1e-6)
  {
    invsLope1 = (ipoint[2].x - ipoint[0].x) / yDiff1;
  }
  else
  {
    // Handle the case where yDiff1 is very small (e.g., set invsLope1 to a very large value or handle it as a vertical line)
    invsLope1 = FLT_MAX;
  }

  // Calculate the difference in y-coordinates for slope 2
  float yDiff2 = (float)ipoint[2].y - (float)ipoint[1].y;
  if (abs(yDiff2) > 1e-6)
  {
    invsLope2 = (ipoint[2].x - ipoint[1].x) / yDiff2;
  }
  else
  {
    // Handle the case where yDiff2 is very small (e.g., set invsLope2 to a very large value or handle it as a vertical line)
    invsLope2 = FLT_MAX;
  }


  if (invsLope1 > invsLope2)
  {
    swap(invsLope1, invsLope2);
  }

  float xStart = (float)ipoint[2].x;
  float xEnd = (float)ipoint[2].x;

  for (int y = (int)ipoint[2].y; y >= (int)ipoint[0].y; --y)
  {
    for (int x = (int)xStart; x >= (int)xEnd; --x)
    {
      SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, color[1]);
    }

    xStart -= invsLope1;
    xEnd -= invsLope2;
  }
}

void SDL_DrawFlatBottomTriangle(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color)
{
  Vec2i ipoint[3] =
  {
    { static_cast<int>(triPoints[0].x), static_cast<int>(triPoints[0].y) },
    { static_cast<int>(triPoints[1].x), static_cast<int>(triPoints[1].y) },
    { static_cast<int>(triPoints[2].x), static_cast<int>(triPoints[2].y) }
  };

  //        P0
  //        /|
  //       / |
  //      /  |
  //     /   |
  //  P1 ---- P2
  //
  //float invsLope1 = (ipoint[1].x - ipoint[0].x) / (float)(ipoint[1].y - ipoint[0].y);
  //float invsLope2 = (ipoint[2].x - ipoint[0].x) / (float)(ipoint[2].y - ipoint[0].y);

  float invsLope1 = 0.0f;
  float invsLope2 = 0.0f;

  // Calculate the difference in y-coordinates for slope 1
  float yDiff1 = (float)ipoint[1].y - (float)ipoint[0].y;
  if (std::abs(yDiff1) > 1e-6)
  {
    invsLope1 = (ipoint[1].x - ipoint[0].x) / yDiff1;
  }
  else
  {
    // Handle the case where yDiff1 is very small (e.g., set invsLope1 to a very large value or handle it as a vertical line)
    invsLope1 = FLT_MAX;
  }

  // Calculate the difference in y-coordinates for slope 2
  float yDiff2 = (float)ipoint[2].y - (float)ipoint[0].y;
  if (std::abs(yDiff2) > 1e-6)
  {
    invsLope2 = (ipoint[2].x - ipoint[0].x) / yDiff2;
  }
  else
  {
    // Handle the case where yDiff2 is very small (e.g., set invsLope2 to a very large value or handle it as a vertical line)
    invsLope2 = FLT_MAX;
  }

  if (invsLope1 > invsLope2)
  {
    swap(invsLope1, invsLope2);
  }

  float xStart = (float)ipoint[0].x;
  float xEnd = (float)ipoint[0].x;

  for (int y = (int)ipoint[0].y; y <= (int)ipoint[1].y; y++)
  {
    for (int x = (int)xStart; x <= (int)xEnd; ++x)
    {
      SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, color[0]);
    }

    xStart += invsLope1;
    xEnd += invsLope2;
  }
}

void SDL_DrawFilledTriangle_FlatTopFlatBottom(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, Vec4f* triPoints, uint32_t* color)
{
  // Make a copy of points
  Vec4f yPointsSorted[3] = { triPoints[0], triPoints[1], triPoints[2] };
  uint32_t colorSorted[3] = { color[0], color[1], color[2] };

  // Sort points vertically
  if (yPointsSorted[0].y > yPointsSorted[1].y)
  {
    swap(yPointsSorted[0], yPointsSorted[1]);
    swap(colorSorted[0], colorSorted[1]);
  }

  if (yPointsSorted[0].y > yPointsSorted[2].y)
  {
    swap(yPointsSorted[0], yPointsSorted[2]);
    swap(colorSorted[0], colorSorted[2]);
  }

  if (yPointsSorted[1].y > yPointsSorted[2].y)
  {
    swap(yPointsSorted[1], yPointsSorted[2]);
    swap(colorSorted[1], colorSorted[2]);
  }

  if ((int)yPointsSorted[1].y == (int)yPointsSorted[2].y)
  {
    // Handle special case triangle with flat bottom only
    //        P0
    //        /|
    //       / |
    //      /  |
    //     /   |
    //  P1 ---- P2
    SDL_DrawFlatBottomTriangle(pRenderBuffer, iBufferWidth, iBufferHight, yPointsSorted, color);
  }
  else if ((int)yPointsSorted[0].y == (int)yPointsSorted[1].y)
  {
    // Handle special case triangle with flat bottom only
    //  P0 ---- P1
    //     \   |
    //      \  |
    //       \ |
    //        p2
    SDL_DrawFlatTopTriangle(pRenderBuffer, iBufferWidth, iBufferHight, yPointsSorted, color);
  }
  else
  {

    // Find triangle base points
    //        P0
    //        /|
    //       / |
    //      /  |
    //     /   |
    //  P1 ---- M
    //     \   |
    //      \  |
    //       \ |
    //        p2

    // Find points M
    Vec4f pointM;
    pointM.y = yPointsSorted[1].y;

    // Calculate the difference in y-coordinates between P2 and P0
    float yDiff = yPointsSorted[2].y - yPointsSorted[0].y;

    // Check if the difference is too small
    if (abs(yDiff) < 1e-6)
    {
      // Handle the case where the difference is very small
      // (e.g., set pointM.x to the average of points[0].x and points[2].x)
      pointM.x = (yPointsSorted[0].x + yPointsSorted[2].x) / 2.0f;
    }
    else
    {
      // Calculate pointM.x as before
      pointM.x = (((yPointsSorted[2].x - yPointsSorted[0].x) * (yPointsSorted[1].y - yPointsSorted[0].y)) / yDiff) + yPointsSorted[0].x;
    }


    Vec4f tempPoint = yPointsSorted[2];
    yPointsSorted[2] = pointM;
    // Draw flat bottom triangle
    SDL_DrawFlatBottomTriangle(pRenderBuffer, iBufferWidth, iBufferHight, yPointsSorted, color);

    yPointsSorted[2] = tempPoint;
    tempPoint = yPointsSorted[0];
    yPointsSorted[0] = yPointsSorted[1];
    yPointsSorted[1] = pointM;

    // draw flat top triangle
    SDL_DrawFlatTopTriangle(pRenderBuffer, iBufferWidth, iBufferHight, yPointsSorted, color);
  }
}

void SDL_DrawTexel(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, float* pDepthBuffer, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight, Vec4f* triPoints, Vec2f* triUVs, int x, int y, Vec3f barycentricCoord)
{
  // Calculate the texture coordinate
  Vec3f uv = { 0.0f, 0.0f, 0.0f };

  // uv.x = barycentricCoord.x * triUVs[0].x + barycentricCoord.y * triUVs[1].x + barycentricCoord.z * triUVs[2].x;
  // uv.y = barycentricCoord.x * triUVs[0].y + barycentricCoord.y * triUVs[1].y + barycentricCoord.z * triUVs[2].y;

  uv.x = barycentricCoord.x * (triUVs[0].x / triPoints[0].w) + barycentricCoord.y * (triUVs[1].x / triPoints[1].w) + barycentricCoord.z * (triUVs[2].x / triPoints[2].w);
  uv.y = barycentricCoord.x * (triUVs[0].y / triPoints[0].w) + barycentricCoord.y * (triUVs[1].y / triPoints[1].w) + barycentricCoord.z * (triUVs[2].y / triPoints[2].w);

  // calculate the reciprocal of w
  // Note: uv.z will hold the reciprocal of w
  // The value will be negative due to projection (matrix)
  uv.z = barycentricCoord.x * (1.0f / triPoints[0].w) + barycentricCoord.y * (1.0f / triPoints[1].w) + barycentricCoord.z * (1.0f / triPoints[2].w);

  // Divide the uv by the reciprocal of w
  uv.x /= uv.z;
  uv.y /= uv.z;

  // Calculate the texture coordinate in the texture space
  uint32_t iTextureX = static_cast<int>(uv.x * iTextureWiedth);
  uint32_t iTextureY = static_cast<int>(uv.y * iTextureHight);

  // Clamp the texture coordinate
  iTextureX = std::clamp(iTextureX, 0u, iTextureWiedth - 1);
  iTextureY = std::clamp(iTextureY, 0u, iTextureHight - 1);

  // Get the texture color
  uint32_t texel = texture[iTextureY * iTextureWiedth + iTextureX];

  if (uv.z < pDepthBuffer[y * iBufferWidth + x])
  {
    // Set the pixel
    SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, texel);
    pDepthBuffer[y * iBufferWidth + x] = uv.z;
  }
}

void SDL_DrawTextured_Barycentric_FlatTopFlatBottom(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, float* pDepthBuffer, Vec4f* triPoints, Vec2f* triUVs, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight)
{
  // Make a copy of points
  // floor the points since those are projected points,
  // screen pixels have no fraction (which could cause error/issues)
  Vec4f yPointsSorted[3] =
  {
    { floor(triPoints[0].x), floor(triPoints[0].y), triPoints[0].z, triPoints[0].w },
    { floor(triPoints[1].x), floor(triPoints[1].y), triPoints[1].z, triPoints[1].w },
    { floor(triPoints[2].x), floor(triPoints[2].y), triPoints[2].z, triPoints[2].w }
  };

  Vec2f uvTexelSorted[3] = { triUVs[0], triUVs[1], triUVs[2] };

  // Sort points vertically
  if (yPointsSorted[0].y > yPointsSorted[1].y)
  {
    swap(yPointsSorted[0], yPointsSorted[1]);
    swap(uvTexelSorted[0], uvTexelSorted[1]);
  }

  if (yPointsSorted[0].y > yPointsSorted[2].y)
  {
    swap(yPointsSorted[0], yPointsSorted[2]);
    swap(uvTexelSorted[0], uvTexelSorted[2]);
  }

  if (yPointsSorted[1].y > yPointsSorted[2].y)
  {
    swap(yPointsSorted[1], yPointsSorted[2]);
    swap(uvTexelSorted[1], uvTexelSorted[2]);
  }

  float invSlope1 = 0;
  float invSlope2 = 0;

  // We calculate the inverse slope (dx/dy) because we are drawing the triangle from top to bottom (y increments 1 every loop)
  if (yPointsSorted[1].y - yPointsSorted[0].y != 0)
  {
    invSlope1 = (float)(yPointsSorted[1].x - yPointsSorted[0].x) / abs(yPointsSorted[1].y - yPointsSorted[0].y);
  }

  if (yPointsSorted[2].y - yPointsSorted[0].y != 0)
  {
    invSlope2 = (float)(yPointsSorted[2].x - yPointsSorted[0].x) / abs(yPointsSorted[2].y - yPointsSorted[0].y);
  }

  if (yPointsSorted[1].y - yPointsSorted[0].y != 0)
  {
    for (int y = (int)yPointsSorted[0].y; y <= (int)yPointsSorted[1].y; y++)
    {
      int xStart = (int)(yPointsSorted[1].x + (y - yPointsSorted[1].y) * invSlope1);
      int xEnd = (int)(yPointsSorted[0].x + (y - yPointsSorted[0].y) * invSlope2);

      if (xEnd < xStart)
      {
        swap(xStart, xEnd);
      }

      for (int x = xStart; x < xEnd; x++)
      {
        Vec3f barycentricCoord; // barycentric coordinate
        Vec4f point { (float)x, (float)y, 0, 0 };
        CalculateBarycentricCoordinate(triPoints, &point, &barycentricCoord);
        SDL_DrawTexel(pRenderBuffer, iBufferWidth, iBufferHight, pDepthBuffer, texture, iTextureWiedth, iTextureHight, triPoints, triUVs, x, y, barycentricCoord);
        // SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, 0xFFFF00FF);
      }
    }
  }

  invSlope1 = 0;
  invSlope2 = 0;

  if (yPointsSorted[2].y - yPointsSorted[1].y != 0)
  {
    invSlope1 = (float)(yPointsSorted[2].x - yPointsSorted[1].x) / abs(yPointsSorted[2].y - yPointsSorted[1].y);
  }

  if (yPointsSorted[2].y - yPointsSorted[0].y != 0)
  {
    invSlope2 = (float)(yPointsSorted[2].x - yPointsSorted[0].x) / abs(yPointsSorted[2].y - yPointsSorted[0].y);
  }

  if (yPointsSorted[2].y - yPointsSorted[1].y != 0)
  {
    for (int y = (int)yPointsSorted[1].y; y <= (int)yPointsSorted[2].y; y++)
    {
      int xStart = (int)(yPointsSorted[1].x + (y - yPointsSorted[1].y) * invSlope1);
      int xEnd = (int)(yPointsSorted[0].x + (y - yPointsSorted[0].y) * invSlope2);

      if (xEnd < xStart)
      {
        swap(xStart, xEnd);
      }

      for (int x = xStart; x < xEnd; x++)
      {
        Vec3f barycentricCoord; // barycentric coordinate
        Vec4f point{ (float)x, (float)y, 0, 0 };
        CalculateBarycentricCoordinate(triPoints, &point, &barycentricCoord);
        SDL_DrawTexel(pRenderBuffer, iBufferWidth, iBufferHight, pDepthBuffer, texture, iTextureWiedth, iTextureHight, triPoints, triUVs, x, y, barycentricCoord);
      }
    }
  }
}

void CalculateBarycentricCoordinate(Vec4f* triPoints, Vec4f* point, Vec3f* outBarycentricCoord)
{

  // Find the vectors between the vertices ABC and point p
  Vec4f ac = triPoints[2] - triPoints[0];
  Vec4f ab = triPoints[1] - triPoints[0];
  Vec4f ap = *point - triPoints[0];
  Vec4f pc = triPoints[2] - *point;
  Vec4f pb = triPoints[1] - *point;

  // Compute the area of the full parallelogram/triangle ABC using 2D cross product
  float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

  // Alpha is the area of the small parallelogram/triangle PBC divided by the area of the full parallelogram/triangle ABC
  float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

  // Beta is the area of the small parallelogram/triangle APC divided by the area of the full parallelogram/triangle ABC
  float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

  // Weight gamma is easily found since barycentric coordinates always add up to 1.0
  float gamma = 1 - alpha - beta;

  outBarycentricCoord->x = alpha;
  outBarycentricCoord->y = beta;
  outBarycentricCoord->z = gamma;


  // Calculate the area of the triangle
  //float area = 0.5f * ((triPoints[1].x - triPoints[0].x) * (triPoints[2].y - triPoints[0].y) - (triPoints[2].x - triPoints[0].x) * (triPoints[1].y - triPoints[0].y));

  // Calculate the barycentric coordinates
  //outBarycentricCoord->x = 0.5f * ((triPoints[1].x * triPoints[2].y - triPoints[2].x * triPoints[1].y) - (triPoints[1].x * point->y - point->x * triPoints[1].y) + (triPoints[2].x * point->y - point->x * triPoints[2].y)) / area;
  //outBarycentricCoord->y = 0.5f * ((triPoints[2].x * triPoints[0].y - triPoints[0].x * triPoints[2].y) - (triPoints[2].x * point->y - point->x * triPoints[2].y) + (triPoints[0].x * point->y - point->x * triPoints[0].y)) / area;
  //outBarycentricCoord->z = 0.5f * ((triPoints[0].x * triPoints[1].y - triPoints[1].x * triPoints[0].y) - (triPoints[0].x * point->y - point->x * triPoints[0].y) + (triPoints[1].x * point->y - point->x * triPoints[1].y)) / area;
}

// Debugging function that would draw a texture to the screen
void Debug_DrawTexture(uint32_t* pRenderBuffer, uint32_t iBufferWidth, uint32_t iBufferHight, uint32_t* texture, uint32_t iTextureWiedth, uint32_t iTextureHight)
{
  // clamp the texture to the screen
  iTextureWiedth = MIN(iTextureWiedth, iBufferWidth);
  iTextureHight = MIN(iTextureHight, iBufferHight);

  for (uint32_t y = 0; y < iTextureHight; y++)
  {
    for (uint32_t x = 0; x < iTextureWiedth; x++)
    {
      SDL_SetPixel(pRenderBuffer, iBufferWidth, iBufferHight, x, y, texture[y * iTextureWiedth + x]);
    }
  }
}