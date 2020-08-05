#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ModelLoader.h"

class PointsCloud : public Scene
{

public:
  PointsCloud();
  void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine) override;
  void ProcessEvent(SDL_Event* event, float deltaTime) override;
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) override;
  void Update(float deltaTime) override;
  void Render() override;
  void Clean() override;

  static std::unique_ptr<Scene> Create()
  {
    return std::make_unique <PointsCloud>();
  }

protected:
  std::unique_ptr<Vec3f[]> m_pPointsCloud;
  std::unique_ptr<Vec3f[]> m_pPointsCloudTransformed;
  Vec3f m_Rotation;
};

