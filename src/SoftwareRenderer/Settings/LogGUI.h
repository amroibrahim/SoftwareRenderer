#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

class LogGUI
{
public:
  LogGUI();
  void Log(const char* format, ...);
  void Render(const char* title, bool* pOpened = NULL);
  void Clear();

protected:
  ImGuiTextBuffer m_logBuffer;
  ImGuiTextFilter m_filter;
  ImVector<int> m_lineOffsets;
  bool m_bScrollBottom;
};
