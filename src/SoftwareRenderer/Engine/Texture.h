#pragma once
#include "SDL_image.h"
#include "Configuration.h"

#include <string>

class Texture
{
public:
  Texture(Configuration* pConfiguration);
  ~Texture();

  void Load(const std::string& path);
  void Unload();

  uint32_t* GetTexture() const;
  int GetWidth() const;
  int GetHeight() const;

protected:
  SDL_Surface* ConvertToRGBA32(SDL_Surface* pSurface);

  Configuration* m_pConfiguration;
  SDL_Surface* m_pSDLSurface;
  SDL_PixelFormat* m_pPixelFormat;
  int m_iWidth;
  int m_iHeight;
};

