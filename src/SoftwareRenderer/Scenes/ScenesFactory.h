#pragma once
#include <map>
#include <string>

#include "Configuration.h"
#include "Scene.h"

class ScenesFactory
{
public:
  explicit ScenesFactory(Configuration* pConfiguration);
  std::string GetSceneList();
  void CreateScene(int iSceneIndex, std::unique_ptr<Scene>& pScene, RendererEngine* pRendererEngine);
  uint32_t GetSceneIndex(std::string& sSceneName);

protected:
  void Register(const std::string& sSceneName, pfnCreate pCreateFunction);

  std::map <std::string, pfnCreate> m_SceneLookup;
  Configuration* m_pConfiguration;
};

