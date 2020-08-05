#include "PointsCloud.h"
#include "RendererEngine.h"
#include "PointsCloudSettingsGUI.h"

#include "Vector3.hpp"

#include <vector>

PointsCloud::PointsCloud()
{
}

void PointsCloud::Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine)
{
  Scene::Initialize(pConfiguration, pRendererEngine);
  m_pSceneSettingsGUI = std::make_unique<PointsCloudSettingsGUI>(pConfiguration);

  m_pPointsCloud = std::make_unique<Vec3f[]>(1000);
  m_pPointsCloudTransformed = std::make_unique<Vec3f[]>(1000);

  m_pConfiguration->render.meshTranslationZ = -20.0f;
  m_pConfiguration->render.renderDrawColorR = 0;
  m_pConfiguration->render.renderDrawColorG = 255;
  m_pConfiguration->render.renderDrawColorB = 0;

  int index = 0;
  // using a 10x10x10 grid of points
  // starting from -5 to 5 so the center of the grid is at 0,0,0
  for (int x = -5; x < 5; ++x)
  {
    for (int y = -5; y < 5; ++y)
    {
      for (int z = -5; z < 5; ++z)
      {
        m_pPointsCloud[index++] = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
      }
    }
  }
}

void PointsCloud::HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime)
{
}

void PointsCloud::Update(float deltaTime)
{
  PointsCloudSettingsGUI* setting = dynamic_cast<PointsCloudSettingsGUI*>(m_pSceneSettingsGUI.get());

  Vec3f cameraPosition = { 0, 0, m_pConfiguration->render.meshTranslationZ };
  m_Rotation.x += setting->GetXRotationAngleSpeed() * deltaTime;
  m_Rotation.y += setting->GetYRotationAngleSpeed() * deltaTime;
  m_Rotation.z += setting->GetZRotationAngleSpeed() * deltaTime;

  for (int i = 0; i < 1000; ++i)
  {
    m_pPointsCloudTransformed[i] = m_pPointsCloud[i];

    // Translate to the center of the screen


    // Rotate the vertex
    m_pPointsCloudTransformed[i] = RotateX(m_pPointsCloudTransformed[i], m_Rotation.x);
    m_pPointsCloudTransformed[i] = RotateY(m_pPointsCloudTransformed[i], m_Rotation.y);
    m_pPointsCloudTransformed[i] = RotateZ(m_pPointsCloudTransformed[i], m_Rotation.z);

    // Translate the vertex away from the camera
    m_pPointsCloudTransformed[i].z -= cameraPosition.z;

    m_pPointsCloudTransformed[i] = m_pRendererEngine->ProjectPoint(m_pPointsCloudTransformed[i]);

    Vec3f center = { m_pConfiguration->display.iScreenBufferWidth / 2.0f, m_pConfiguration->display.iScreenBufferHeight / 2.0f, 0 };
    m_pPointsCloudTransformed[i] = m_pPointsCloudTransformed[i] + center;
  }
}

void PointsCloud::Render()
{
  m_pRendererEngine->Render(m_pPointsCloudTransformed.get(), 1000);
}

void PointsCloud::Clean()
{
}

void PointsCloud::ProcessEvent(SDL_Event* event, float deltaTime)
{
}
