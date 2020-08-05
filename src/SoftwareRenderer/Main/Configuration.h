#pragma once

#include <SDL.h>
#include <cstdint>

#include "LogGUI.h"

// Display settings
struct Display
{
  int iScreenBufferWidth = 1920;
  int iScreenBufferHeight = 1080;

  int iFrameRate = 60;

  // SDL_Window* window;
};

// Render settings
struct Render
{
  uint8_t renderClearColorR = 0;
  uint8_t renderClearColorG = 0;
  uint8_t renderClearColorB = 0;

  uint8_t renderDrawColorR = 255;
  uint8_t renderDrawColorG = 255;
  uint8_t renderDrawColorB = 255;

  uint8_t renderWireframeColorR = 255;
  uint8_t renderWireframeColorG = 255;
  uint8_t renderWireframeColorB = 255;

  uint8_t gridColorR = 20;
  uint8_t gridColorG = 20;
  uint8_t gridColorB = 20;

  float fovFactor = 128 * 6;

  bool renderGrid = true;

  bool renderWireframe = true;

  bool renderFillTriangles = false;

  bool renderTexturedTriangles = false;

  bool enableTriangleSorting = true;

  int gridSpacing = 10;

  // 90 degrees to radians
  float fov = (float)M_PI / 3.0f;
  float nearPlane = 1.0f;
  float farPlane = 100.0f;

  float meshTranslationZ = 5.0f;
  float maxTranslateZ = 100.0f;

  SDL_Renderer* renderer = nullptr;
  SDL_Surface* mainSurface = nullptr;
};

struct Configuration
{
  Display display;
  Render render;
  LogGUI log;
};
