#pragma once

#include <SDL.h>

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

#include "Configuration.h"
#include "SceneSettingsGUI.h"

class SettingsGUI
{
public:
  explicit SettingsGUI(Configuration* configuration);

  void Initialize(SDL_Window* window, SDL_Renderer* renderer, std::string scenesList);
  void ReloadSettings();
  void ProcessEvent(SDL_Event* event);
  void Update();
  void Render(SceneSettingsGUI* sceneGUI);
  void Clean();
  void SetSceneSelection(int sceneIndex);
  int GetSceneSelection();
  bool IsGridSettingsEnabled();
  bool IsCapturingMouseKeyboard();


protected:
  void MainSettingsWindow();
  void ShowWarningPopup();

  bool m_bShowLogWindow;
  bool m_bEnableGridSettings;
  bool m_bShowWarningPopup;
  int m_iSceneSelection;
  std::string m_sScenesList;
  Configuration* m_pConfiguration;
  ImVec4 m_clearColor;
  ImVec4 m_drawColor;
  ImVec4 m_gridColor;
};

