#pragma once

#include <SDL.h>

#include "Configuration.h"
#include "SDLWindow.h"
#include "RendererEngine.h"
#include "ScenesFactory.h"
#include "Scene.h"

class SoftwareRenderer
{

public:
  SoftwareRenderer();
  virtual ~SoftwareRenderer();

  void Initialize();
  void Run();
  void Clean();

protected:
  bool IsOver();
  void InitializeFrame();
  void ProcessInput();
  void Render();
  void Update();
  void Delay();
  void Quit();

  void SwitchSceneOnSelection();
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates);

  bool m_bIsOver;
  int m_iCurrentSceneIndex;
  int m_iWindowPositionX;
  int m_iWindowPositionY;

  uint32_t m_frameLength;
  uint32_t m_lastFrameTime;
  float m_deltaTime;

  Configuration m_Configuration;

  SettingsGUI m_SettingsGUI;

  std::unique_ptr<SDLWindow> m_pSDLWindow;
  std::unique_ptr <RendererEngine> m_pRendererEngine;
  std::unique_ptr<Scene> m_pCurrentScene;

  ScenesFactory m_ScenesFactory;
};

