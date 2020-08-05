#include "Triangle.h"
#include "RendererEngine.h"
#include "TriangleSettingsGUI.h"
#include "Vector3.hpp"

#include <vector>

Triangle::Triangle()
{
}

void Triangle::Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine)
{
  Scene::Initialize(pConfiguration, pRendererEngine);
  m_pSceneSettingsGUI = std::make_unique<TriangleSettingsGUI>(pConfiguration);

  //m_pTriangle = std::make_unique<Vec3f[]>(3);
  //m_pTriangleTransformed = std::make_unique<Vec3f[]>(3);

  m_pConfiguration->render.meshTranslationZ = -20.0f;
  m_pConfiguration->render.renderDrawColorR = 0;
  m_pConfiguration->render.renderDrawColorG = 255;
  m_pConfiguration->render.renderDrawColorB = 0;


  m_TrianglePoints[0] = { -6.0f, -4.0f, 0.0f };
  m_TrianglePoints[1] = { 2.0f, -7.0f, 0.0f };
  m_TrianglePoints[2] = { -4.0f, 4.0f, 0.0f };

  TriangleSettingsGUI* setting = dynamic_cast<TriangleSettingsGUI*>(m_pSceneSettingsGUI.get());
  setting->SetPoint1(m_TrianglePoints[0]);
  setting->SetPoint2(m_TrianglePoints[1]);
  setting->SetPoint3(m_TrianglePoints[2]);
}

void Triangle::HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime)
{

}

void Triangle::Update(float deltaTime)
{
  TriangleSettingsGUI* setting = dynamic_cast<TriangleSettingsGUI*>(m_pSceneSettingsGUI.get());

  m_bEnableFilling = setting->IsFillingEnabled();
  m_TrianglePoints[0] = setting->GetPoint1();
  m_TrianglePoints[1] = setting->GetPoint2();
  m_TrianglePoints[2] = setting->GetPoint3();

  m_PointsColors[0] = setting->m_ColorConverterPoint1.color;
  m_PointsColors[1] = setting->m_ColorConverterPoint2.color;
  m_PointsColors[2] = setting->m_ColorConverterPoint3.color;

  for (int i = 0; i < 3; ++i)
  {
    m_TrianglePointsTransformed[i] = m_TrianglePoints[i];

    // Translate the vertex away from the camera
    m_TrianglePointsTransformed[i].z -= m_pConfiguration->render.meshTranslationZ;

    m_TrianglePointsTransformed[i] = m_pRendererEngine->ProjectPoint(m_TrianglePointsTransformed[i]);

    Vec3f center = { m_pConfiguration->display.iScreenBufferWidth / 2.0f, m_pConfiguration->display.iScreenBufferHeight / 2.0f, 0 };
    m_TrianglePointsTransformed[i] = m_TrianglePointsTransformed[i] + center;
  }
}

void Triangle::Render()
{
  m_pRendererEngine->RenderTriangle(m_TrianglePointsTransformed, m_PointsColors, m_bEnableFilling);
}

void Triangle::Clean()
{
}

void Triangle::ProcessEvent(SDL_Event* event, float deltaTime)
{
}
