#pragma once

#include <memory>

#include <SDL.h>
#include "Configuration.h"
#include "SceneSettingsGUI.h"
#include "RendererEngine.h"

class Scene
{
public:
  Scene() : m_pSceneSettingsGUI(nullptr), m_pConfiguration(nullptr), m_pRendererEngine(nullptr) {}
  virtual void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine)
  {
    m_pConfiguration = pConfiguration;
    m_pRendererEngine = pRendererEngine;
  }

  virtual void ProcessEvent(SDL_Event* event, float deltaTime) = 0;
  virtual void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) = 0;
  virtual void Update(float deltaTime) = 0;
  virtual void Render() = 0;
  virtual void Clean() = 0;

  std::unique_ptr<SceneSettingsGUI> m_pSceneSettingsGUI;

protected:
  Configuration* m_pConfiguration;
  RendererEngine* m_pRendererEngine;

};

typedef std::unique_ptr<Scene>(*pfnCreate)(void);