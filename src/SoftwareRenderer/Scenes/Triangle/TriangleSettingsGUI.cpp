#include "TriangleSettingsGUI.h"

TriangleSettingsGUI::TriangleSettingsGUI(Configuration* configuration) :  m_pConfiguration(configuration), SceneSettingsGUI("Triangle Settings")
{
  m_minCameraPosition = 0;
  m_maxCameraPosition = -100.0f;

  m_minRotationAngle = -20.0f;
  m_maxRotationAngle = 20.0f;

  m_point1[0] = 0.0f;
  m_point1[1] = 0.0f;

  m_point2[0] = 0.0f;
  m_point2[1] = 0.0f;

  m_point3[0] = 0.0f;
  m_point3[0] = 0.0f;

  m_Point1Color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
  m_Point2Color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
  m_Point3Color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue

  m_bEnableFilling = false;
}

void TriangleSettingsGUI::Render()
{
  ImGui::Begin(m_kTitle);
  // ImGui::Text("CubeSettingsGUI");
  ImGui::SliderFloat("Camera Position", &m_pConfiguration->render.meshTranslationZ, m_minCameraPosition, m_maxCameraPosition);
  ImGui::SliderFloat2("Point 1 (x, y)", m_point1, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat2("Point 2 (x, y)", m_point2, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat2("Point 3 (x, y)", m_point3, m_minRotationAngle, m_maxRotationAngle, "%.1f");

  ImGui::Checkbox("Enable triangle filling", &m_bEnableFilling);

  ImGui::ColorEdit3("Point 1 Color", reinterpret_cast<float*>(&m_Point1Color));
  ImGui::ColorEdit3("Point 2 Color", reinterpret_cast<float*>(&m_Point2Color));
  ImGui::ColorEdit3("Point 3 Color", reinterpret_cast<float*>(&m_Point3Color));

  m_ColorConverterPoint1.r = static_cast<uint8_t>(m_Point1Color.x * 255);
  m_ColorConverterPoint1.g = static_cast<uint8_t>(m_Point1Color.y * 255);
  m_ColorConverterPoint1.b = static_cast<uint8_t>(m_Point1Color.z * 255);

  m_ColorConverterPoint2.r = static_cast<uint8_t>(m_Point2Color.x * 255);
  m_ColorConverterPoint2.g = static_cast<uint8_t>(m_Point2Color.y * 255);
  m_ColorConverterPoint2.b = static_cast<uint8_t>(m_Point2Color.z * 255);

  m_ColorConverterPoint3.r = static_cast<uint8_t>(m_Point3Color.x * 255);
  m_ColorConverterPoint3.g = static_cast<uint8_t>(m_Point3Color.y * 255);
  m_ColorConverterPoint3.b = static_cast<uint8_t>(m_Point3Color.z * 255);

  ImGui::End();
}

void TriangleSettingsGUI::SetPoint1(Vec3f point)
{
  m_point1[0] = point.x;
  m_point1[1] = point.y;
}

void TriangleSettingsGUI::SetPoint2(Vec3f point)
{
  m_point2[0] = point.x;
  m_point2[1] = point.y;
}

void TriangleSettingsGUI::SetPoint3(Vec3f point)
{
  m_point3[0] = point.x;
  m_point3[1] = point.y;
}

Vec3f TriangleSettingsGUI::GetPoint1()
{
  return { m_point1[0], m_point1[1], 0.0f };
}

Vec3f TriangleSettingsGUI::GetPoint2()
{
  return { m_point2[0], m_point2[1], 0.0f };
}

Vec3f TriangleSettingsGUI::GetPoint3()
{
  return { m_point3[0], m_point3[1], 0.0f };
}

bool TriangleSettingsGUI::IsFillingEnabled() const
{
  return m_bEnableFilling;
}
