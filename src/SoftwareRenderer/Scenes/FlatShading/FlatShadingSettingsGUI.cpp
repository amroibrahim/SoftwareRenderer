#include "FlatShadingSettingsGUI.h"

FlatShadingSettingsGUI::FlatShadingSettingsGUI(Configuration* configuration) : m_pConfiguration(configuration), SceneSettingsGUI("Wireframe Settings"), m_bEnableCulling(false),
  m_bEnableFillTriangles(false), m_bEnableWireframe(true)
{
  m_minTranslateZ = 0;
  m_maxTranslatePosition = 100.0f;

  m_minRotationAngle = 0;
  m_maxRotationAngle = 1.0f;

  m_xRotationAngleSpeed = 0.5f;
  m_yRotationAngleSpeed = 0.5f;
  m_zRotationAngleSpeed = 0.5f;

  m_WireframeColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default wire frame color (white)
}

void FlatShadingSettingsGUI::Render()
{
  ImGui::Begin(m_kTitle);
  ShowModelsListSelector();
  ImGui::Checkbox("Draw filled triangles", &m_pConfiguration->render.renderFillTriangles);
  ImGui::Checkbox("Draw wire frame triangles", &m_pConfiguration->render.renderWireframe);

  if (!m_pConfiguration->render.renderWireframe)
  {
    ImGui::BeginDisabled();
  }
  ImGui::ColorEdit3("Wireframe Color", reinterpret_cast<float*>(&m_WireframeColor));
  if (!m_pConfiguration->render.renderWireframe)
  {
    ImGui::EndDisabled();
  }

  ImGui::Checkbox("Enable back face culling", &m_bEnableCulling);
  ImGui::Checkbox("Enable triangle sorting", &m_pConfiguration->render.enableTriangleSorting);
  ImGui::SliderFloat("Translate Model", &m_pConfiguration->render.meshTranslationZ, m_minTranslateZ, m_pConfiguration->render.maxTranslateZ);
  ImGui::SliderFloat("X Rotation Speed", &m_xRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat("Y Rotation Speed", &m_yRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");
  ImGui::SliderFloat("Z Rotation Speed", &m_zRotationAngleSpeed, m_minRotationAngle, m_maxRotationAngle, "%.1f");

  if (ImGui::Button("Reset Rotation"))
  {
    m_bResetRotation = true;
  }

  ImGui::End();

  // Update the configuration with the wire frame color
  m_pConfiguration->render.renderWireframeColorR = static_cast<uint8_t>(m_WireframeColor.x * 255);
  m_pConfiguration->render.renderWireframeColorG = static_cast<uint8_t>(m_WireframeColor.y * 255);
  m_pConfiguration->render.renderWireframeColorB = static_cast<uint8_t>(m_WireframeColor.z * 255);
}

void FlatShadingSettingsGUI::SetModelsList(const std::vector<std::string>& filesList)
{
  m_ModelsList = filesList;
}

float FlatShadingSettingsGUI::GetXRotationAngleSpeed() const
{
  return m_xRotationAngleSpeed;
}

float FlatShadingSettingsGUI::GetYRotationAngleSpeed() const
{
  return m_yRotationAngleSpeed;
}

float FlatShadingSettingsGUI::GetZRotationAngleSpeed() const
{
  return m_zRotationAngleSpeed;
}

bool FlatShadingSettingsGUI::IsCullingEnabled() const
{
  return m_bEnableCulling;
}

bool FlatShadingSettingsGUI::IsResetRotation()
{
  bool reset = m_bResetRotation;
  m_bResetRotation = false; // Reset the flag after checking
  return reset;
}

std::string FlatShadingSettingsGUI::GetSelectedModel()
{
  return m_sSelectedModel;
}

void FlatShadingSettingsGUI::ShowModelsListSelector()
{
  if (ImGui::BeginCombo("Select File", m_ModelsList.empty() ? "Select a model" : m_sSelectedModel.c_str()))
  {
    for (int i = 0; i < m_ModelsList.size(); i++)
    {
      bool isSelected = (m_sSelectedModel == m_ModelsList[i]);
      if (ImGui::Selectable(m_ModelsList[i].c_str(), isSelected))
      {
        m_sSelectedModel = m_ModelsList[i];  // Set the selected file
      }

      // Set the initial focus when opening the combo (optional)
      if (isSelected)
      {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}

void FlatShadingSettingsGUI::SetSelectedModel(const std::string& fileName)
{
  m_sSelectedModel = fileName;
}

void FlatShadingSettingsGUI::SetCullingEnabled(bool enabled)
{
  m_bEnableCulling = enabled;
}
