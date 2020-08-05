#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "Light.h"

class FlatShading : public Scene
{

public:
  FlatShading();
  void Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine) override;

  void LoadMesh();

  void ProcessEvent(SDL_Event* event, float deltaTime) override;
  void HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime) override;
  void Update(float deltaTime) override;
  void Render() override;
  void Clean() override;

  static std::unique_ptr<Scene> Create()
  {
    return std::make_unique <FlatShading>();
  }

protected:
  void UpdateMesh(float deltaTime);

  Light m_Light;
  std::unique_ptr<Mesh> m_pModel;
  ModelLoader m_Loader;
  std::string m_sSelectedModel;

};

