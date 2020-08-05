#include "ScenesFactory.h"

#include "Empty\EmptyScene.h"
#include "PointsCloud\PointsCloud.h"
#include "Triangle\Triangle.h"
#include "Wireframe\Wireframe.h"
#include "FlatShading\FlatShading.h"
#include "TextureMapping\TextureMapping.h"
#include "FPSCamera\FPSCamera.h"

ScenesFactory::ScenesFactory(Configuration* pConfiguration)
{
  m_pConfiguration = pConfiguration;
  Register("00 - Empty", &EmptyScene::Create);
  Register("01 - Points Cloud", &PointsCloud::Create);
  Register("02 - Triangle", &Triangle::Create);
  Register("03 - Wireframe", &Wireframe::Create);
  Register("04 - Flat Shading", &FlatShading::Create);
  Register("05 - Texture Mapping", &TextureMapping::Create);
  Register("06 - FPS Camera", &FPSCamera::Create);
}

std::string ScenesFactory::GetSceneList()
{
  std::string list;

  for (std::map <std::string, pfnCreate>::iterator itr = m_SceneLookup.begin();
       itr != m_SceneLookup.end();
       itr++)
  {
    list += itr->first;
    list.push_back('\0');
  }

  list.push_back('\0');
  return list;
}

void ScenesFactory::CreateScene(int sceneIndex, std::unique_ptr<Scene>& pScene, RendererEngine* pRendererEngine)
{
  std::map <std::string, pfnCreate>::iterator itr = m_SceneLookup.begin();
  std::advance(itr, sceneIndex);
  std::string sceneName = itr->first;

  itr = m_SceneLookup.find(sceneName);

  if (itr != m_SceneLookup.end())
  {
    pScene = std::move(itr->second());
  }
  else
  {
    pScene = std::move(m_SceneLookup["00 - Empty"]());
  }

  pScene->Initialize(m_pConfiguration, pRendererEngine);
}

uint32_t ScenesFactory::GetSceneIndex(std::string& sceneName)
{
  return static_cast<uint32_t>(distance(m_SceneLookup.begin(), m_SceneLookup.find(sceneName)));
}

void ScenesFactory::Register(const std::string& sSceneName, pfnCreate pCreateFunction)
{
  m_SceneLookup[sSceneName] = pCreateFunction;
}
