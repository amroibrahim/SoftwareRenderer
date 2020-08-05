#include "PointsCloudSettingsGUI.h"

PointsCloudSettingsGUI::PointsCloudSettingsGUI(Configuration* configuration) :  m_pConfiguration(configuration), SceneSettingsGUI("Cube Settings")
{
  m_minCameraPosition = 0;
  m_maxCameraPosition = -100.0f;

  m_minRotationAngle = 0;
  m_maxRotationAngle = 1.0f;

  m_xRotationAngleSpeed = 0.5f;
  m_yRotationAngleSpeed = 0.5f;
  m_zRotationAngleSpeed = 0.5f;
}

void PointsCloudSettingsGUI::Render()
{
  ImGui::Begin(m_kTitle);
  // ImGui::Text("CubeSettingsGUI");
  ImGui::SliderFloat("Camera Position", &m_pConfiguration->render.meshTranslationZ, m_minCameraPosition, m_maxCameraPosition);
  ImGui::SliderFloat("X Rotation Speed", &m_xRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat("Y Rotation Speed", &m_yRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat("Z Rotation Speed", &m_zRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::End();
}

float PointsCloudSettingsGUI::GetXRotationAngleSpeed()
{
  return m_xRotationAngleSpeed;
}

float PointsCloudSettingsGUI::GetYRotationAngleSpeed()
{
  return m_yRotationAngleSpeed;
}

float PointsCloudSettingsGUI::GetZRotationAngleSpeed()
{
  return m_zRotationAngleSpeed;
}
