#include "SDLWindow.h"
#include <iostream>

SDLWindow::SDLWindow() : m_iScreenWidth(100), m_iScreenHeight(100), m_pSDLWindow(nullptr), m_pSDLRenderer(nullptr), m_pConfiguration(nullptr)
{
}

SDLWindow::~SDLWindow()
{
  SDL_DestroyTexture(m_pRenderBufferTexture);
  SDL_DestroyRenderer(m_pSDLRenderer);
  SDL_DestroyWindow(m_pSDLWindow);
  SDL_Quit();
}

bool SDLWindow::Initialize(Configuration* pConfiguration, const std::string& sWindowTitle)
{
  m_pConfiguration = pConfiguration;

  m_iScreenWidth = m_pConfiguration->display.iScreenBufferWidth;
  m_iScreenHeight = m_pConfiguration->display.iScreenBufferHeight;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }

  m_pSDLWindow = SDL_CreateWindow(sWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_iScreenWidth, m_iScreenHeight, SDL_WINDOW_SHOWN);
  if (m_pSDLWindow == nullptr)
  {
    std::cout << "SDL failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }

  m_pSDLRenderer = SDL_CreateRenderer(m_pSDLWindow, -1, SDL_RENDERER_SOFTWARE);
  if (m_pSDLRenderer == nullptr)
  {
    std::cout << "SDL failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }
  // Note: There is a difference between SDL_PIXELFORMAT_RGBA8888 and SDL_PIXELFORMAT_RGBA32, endianness plays a roll in the format
  m_pRenderBufferTexture = SDL_CreateTexture(m_pSDLRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_iScreenWidth, m_iScreenHeight);
  if (m_pRenderBufferTexture == nullptr)
  {
    std::cout << "SDL failed to create texture! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_SetRenderDrawColor(m_pSDLRenderer, 0, 0, 0, 255);
  SDL_RenderClear(m_pSDLRenderer);

  return true;
}

SDL_Renderer* SDLWindow::GetRenderer()
{
  return m_pSDLRenderer;
}

SDL_Window* SDLWindow::GetWindows()
{
  return m_pSDLWindow;
}

SDL_Surface* SDLWindow::CreateSurface()
{
  int width = m_pConfiguration->display.iScreenBufferWidth;
  int hight = m_pConfiguration->display.iScreenBufferHeight;

  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  return SDL_CreateRGBSurface(0, width, hight, 32, rmask, gmask, bmask, amask);
}

void SDLWindow::Render()
{
  SDL_RenderPresent(m_pSDLRenderer);
}

void SDLWindow::Delay()
{
  SDL_Delay(1000 / 60);
}

void SDLWindow::BlitBuffer(uint32_t* pRenderBuffer)
{
  int iSuccess = SDL_UpdateTexture(m_pRenderBufferTexture, nullptr, pRenderBuffer, m_iScreenWidth * sizeof(uint32_t));
  if (iSuccess != 0)
  {
    std::cout << "SDL failed to update RGB texture! SDL_Error: " << SDL_GetError() << std::endl;
    return;
  }

  iSuccess = SDL_RenderCopy(m_pSDLRenderer, m_pRenderBufferTexture, nullptr, nullptr);
  if (iSuccess != 0)
  {
    std::cout << "SDL failed to copy RGB texture! SDL_Error: " << SDL_GetError() << std::endl;
    return;
  }
}

