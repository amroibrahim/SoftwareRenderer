#pragma once
#include "Scene.h"

class EmptyScene : public Scene
{

public:
  void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine) override;
  void ProcessEvent(SDL_Event* event, float deltaTime) override;
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) override;
  void Update(float deltaTime) override;
  void Render() override;
  void Clean() override;

  static std::unique_ptr<Scene> Create()
  {
    return std::make_unique <EmptyScene>();
  }
};

