// SDL Draw

#ifndef SDL_DRAW_H
#define SDL_DRAW_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set pixel to a buffer.
 *
 * The x and y point coordinates should be uint32_t.
 * point outside the buffer will be ignored.
 *
 * \param pRenderBuffer point to the buffer to be draw the line
 * \param x, y point coordinates
 * \param color color to be used in drawing
 */
void SDL_SetPixel(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x, uint32_t y, uint32_t color);

enum SDL_DRAW_LINE_ALGORITHM
{
  LINE_DDA,
  LINE_BRESENHAM
};

void SDL_DrawLine(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

/**
 * Draw line to a buffer using digital differential analyzer algorithm.
 *
 * The x and y point coordinates should be uint32_t.
 * line point outside the buffer will be ignored.
 *
 * \param pRenderBuffer point to the buffer to be draw the line
 * \param x1, y1 starting point coordinates
 * \param x2, y2 end point coordinates
 * \param color color to be used in drawing
 */
void SDL_DrawLine_DDA(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

/**
 * Draw line to a buffer using Bresenham algorithm.
 * Based on https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm
 *
 * The x and y point coordinates should be uint32_t.
 * line point outside the buffer will be ignored.
 *
 * \param pRenderBuffer point to the buffer to be draw the line
 * \param x1, y1 starting point coordinates
 * \param x2, y2 end point coordinates
 * \param color color to be used in drawing
 *
 */
void SDL_DrawLine_Bresenham(uint32_t* pRenderBuffer, uint32_t iWidth, uint32_t iHight, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

#ifdef __cplusplus
} //extern "C"
#endif

#endif  // SDL_DRAW_H
