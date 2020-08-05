#pragma once

#include <SDL.h>
#include <string>
#include <vector>

#include "RendererEngine.h"
#include "SettingsGUI.h"

class SDLWindow
{
public:
  SDLWindow();
  ~SDLWindow();

  bool Initialize(Configuration* pConfiguration, const std::string& sWindowTitle);

  void Render();
  void Delay();
  void BlitBuffer(uint32_t* pRenderBuffer);

  SDL_Renderer* GetRenderer();
  SDL_Window* GetWindows();
  SDL_Surface* CreateSurface();


protected:
  int m_iScreenWidth;
  int m_iScreenHeight;

  Configuration* m_pConfiguration;

  SDL_Window* m_pSDLWindow;
  SDL_Renderer* m_pSDLRenderer;
  SDL_Texture* m_pRenderBufferTexture;
};

