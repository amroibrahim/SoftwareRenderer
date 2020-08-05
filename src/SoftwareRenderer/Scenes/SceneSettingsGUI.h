#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

class SceneSettingsGUI
{
public:
  explicit SceneSettingsGUI(const char* title) : m_kTitle(title) {}
  virtual void Render() = 0;

protected:
  const char* m_kTitle;
};
