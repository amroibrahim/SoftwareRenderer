#pragma once
#include "Scene.h"
#include "ModelLoader.h"

class Triangle : public Scene
{

public:
  Triangle();
  void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine) override;
  void ProcessEvent(SDL_Event* event, float deltaTime) override;
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) override;
  void Update(float deltaTime) override;
  void Render() override;
  void Clean() override;

  static std::unique_ptr<Scene> Create()
  {
    return std::make_unique <Triangle>();
  }

protected:
  Vec3f m_TrianglePoints[3];
  Vec3f m_TrianglePointsTransformed[3];
  Vec3f m_Rotation;
  uint32_t m_PointsColors[3];
  bool m_bEnableFilling;
};

