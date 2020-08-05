#include "Texture.h"
#include <iostream>

Texture::Texture(Configuration* pConfiguration)
{
  m_pConfiguration = pConfiguration;
  m_pSDLSurface = nullptr;
  m_iWidth = 0;
  m_iHeight = 0;
}

Texture::~Texture()
{
  Unload();
}

void Texture::Load(const std::string& path)
{
  Unload();

  m_pSDLSurface = IMG_Load(path.c_str());
  if (m_pSDLSurface == nullptr)
  {
    std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
    return;
  }

  // Convert surface format if needed to match SDL Window texture format
  m_pSDLSurface = ConvertToRGBA32(m_pSDLSurface);

  std::cout << "Texture load: " << path.c_str() << std::endl;

  m_iWidth = m_pSDLSurface->w;
  m_iHeight = m_pSDLSurface->h;
  m_pPixelFormat = m_pSDLSurface->format;

  std::cout << "Texture pixel format: " << m_pPixelFormat->format << std::endl;
}

void Texture::Unload()
{
  if (m_pSDLSurface != nullptr)
  {
    SDL_FreeSurface(m_pSDLSurface);
    m_pSDLSurface = nullptr;
    m_iWidth = 0;
    m_iHeight = 0;
  }
}

uint32_t* Texture::GetTexture() const
{
  if (m_pSDLSurface == nullptr)
    return nullptr;

  return (uint32_t*)m_pSDLSurface->pixels;
}

int Texture::GetWidth() const
{
  return m_iWidth;
}

int Texture::GetHeight() const
{
  return m_iHeight;
}

SDL_Surface* Texture::ConvertToRGBA32(SDL_Surface* pSurface)
{
  if (pSurface == NULL)
  {
    return NULL;
  }

  // Check if the surface is already in the desired format
  if (pSurface->format->format == SDL_PIXELFORMAT_RGBA32)
  {
    return pSurface; // No conversion needed
  }

  // Create a new surface with the desired format
  SDL_Surface* pConvertedSurface = SDL_ConvertSurfaceFormat(pSurface, SDL_PIXELFORMAT_RGBA32, 0);

  if (pConvertedSurface == NULL)
  {
    // Handle error
  }

  // Free the original surface (optional, depending on your needs)
  SDL_FreeSurface(pSurface);

  return pConvertedSurface;
}

