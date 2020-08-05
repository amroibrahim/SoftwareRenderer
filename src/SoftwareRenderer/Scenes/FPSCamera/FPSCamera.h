#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "Light.h"
#include "Camera.h"
#include "Clipping.h"

#include <memory>

class FPSCamera : public Scene
{

public:
  FPSCamera();
  void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine) override;

  void LoadMesh();

  void ProcessEvent(SDL_Event* event, float deltaTime) override;
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) override;
  void Update(float deltaTime) override;
  void Render() override;
  void Clean() override;

  static std::unique_ptr<Scene> Create()
  {
    return std::make_unique <FPSCamera>();
  }

protected:
  void UpdateMesh(float deltaTime);

  void Debug_LoadMesh();

  Light m_Light;
  std::unique_ptr<Mesh> m_pModel;
  ModelLoader m_Loader;
  std::string m_sSelectedModel;
  Camera m_Camera;

  int m_iPreviousMouseX;
  int m_iPreviousMouseY;

  Clipping m_Clipping;
};

