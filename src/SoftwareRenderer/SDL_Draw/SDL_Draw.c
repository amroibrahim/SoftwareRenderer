// SDL Draw

#include "SDL_Draw.h" 

// #define BOUNDARY_CHECK_ENABLED 

#define ROUND(n) ((int) (n + 0.5))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

enum SDL_DRAW_LINE_ALGORITHM SDL_DRAW_LINE_ALGO = LINE_BRESENHAM;

void SDL_SetPixel(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x, uint32_t y, uint32_t color)
{
#ifdef BOUNDARY_CHECK_ENABLED
  if (x < 0 || iWidth <= x ||
    y1 < 0 || iHight <= y)
  {
    return;
  }
#endif
  uint32_t* pixels = pRenderBuffer;

  if (0 <= x && x < iWidth &&
    0 <= y && y < iHight)
  {
    pixels[(y * iWidth) + x] = color;
  }
}

void SDL_DrawLine(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
#ifdef BOUNDARY_CHECK_ENABLED
  if (x1 < 0 || iWidth <= x1 ||
    x2 < 0 || iWidth <= x2 ||
    y1 < 0 || iHight <= y1 ||
    y2 < 0 || iHight <= y2)
  {
    return;
  }
#endif

  switch (SDL_DRAW_LINE_ALGO)
  {
  case LINE_DDA:
    SDL_DrawLine_DDA(pRenderBuffer, iWidth, iHight, x1, y1, x2, y2, color);
    break;

  case LINE_BRESENHAM:
    SDL_DrawLine_Bresenham(pRenderBuffer, iWidth, iHight, x1, y1, x2, y2, color);
    break;
  }
}

void SDL_DrawLine_DDA(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  int x_delta = x2 - x1;
  int y_delta = y2 - y1;

  int steps = MAX(abs(x_delta), abs(y_delta));

  if (steps <= 0)
  {
    return;
  }

  float x_current = (float)x1;
  float y_current = (float)y1;

  float x_incremant = (float)x_delta / (float)steps;
  float y_incremant = (float)y_delta / (float)steps;

  uint32_t* pixels = pRenderBuffer;

  int step = 0;
  do
  {
    if (0 <= x_current && x_current < iWidth &&
      0 <= y_current && y_current < iHight)
    {
      pixels[(ROUND(y_current) * iWidth) + ROUND(x_current)] = color;
    }

    x_current += x_incremant;
    y_current += y_incremant;
    ++step;

  } while (step <= steps);
}

void clamp(uint32_t* x, uint32_t clampStart, uint32_t clampEnd)
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

void SDL_DrawLine_Bresenham(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  clamp(&x1, 0, iWidth - 1);
  clamp(&x2, 0, iWidth - 1);
  clamp(&y1, 0, iHight - 1);
  clamp(&y2, 0, iHight - 1);

  int x_delta = abs(x2 - x1);
  int y_delta = abs(y2 - y1);

  int x_incremant = x1 < x2 ? 1 : -1;
  int y_incremant = y1 < y2 ? 1 : -1;

  int x_current = x1;
  int y_current = y1;

  int err = (x_delta > y_delta ? x_delta : -y_delta) / 2;

  uint32_t* pixels = pRenderBuffer;

  pixels[(y_current * iWidth) + x_current] = color;

  while (x_current != x2 || y_current != y2)
  {
    int err_current = err;
    if (err_current > -x_delta)
    {
      err -= y_delta;
      x_current += x_incremant;
    }

    if (err_current < y_delta)
    {
      err += x_delta;
      y_current += y_incremant;
    }

    pixels[(y_current * iWidth) + x_current] = color;
  };
}
