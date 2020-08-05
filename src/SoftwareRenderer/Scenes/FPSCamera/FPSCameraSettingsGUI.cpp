#include "FPSCameraSettingsGUI.h"

FPSCameraSettingsGUI::FPSCameraSettingsGUI(Configuration* configuration) : m_pConfiguration(configuration), SceneSettingsGUI("Wireframe Settings"), m_bEnableCulling(false),
  m_bEnableFillTriangles(false), m_bEnableWireframe(true)
{
  m_minTranslateZ = 0;
  m_maxTranslatePosition = 100.0f;

  m_minCameraPositionIncrement = -5.0f;
  m_maxCameraPositionIncrement = 5.0f;

  m_xCameraPositionIncrement = 0.0f;
  m_yCameraPositionIncrement = 0.0f;
  m_zCameraPositionIncrement = 0.0f;

  m_WireframeColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default wire frame color (white)

  m_bShowHintsPopup = true;
}

void FPSCameraSettingsGUI::Render()
{
  ImGui::Begin(m_kTitle);
  ShowModelsListSelector();

  bool drawFilled = m_pConfiguration->render.renderFillTriangles;
  bool drawTextured = m_pConfiguration->render.renderTexturedTriangles;

  if (ImGui::Checkbox("Draw filled triangles", &drawFilled))
  {
    m_pConfiguration->render.renderFillTriangles = drawFilled;
    if (drawFilled)
    {
      m_pConfiguration->render.renderTexturedTriangles = false; // Uncheck the other
    }
  }

  if (ImGui::Checkbox("Draw textured triangles", &drawTextured))
  {
    m_pConfiguration->render.renderTexturedTriangles = drawTextured;
    if (drawTextured)
    {
      m_pConfiguration->render.renderFillTriangles = false; // Uncheck the other
    }
  }

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
  ImGui::SliderFloat("X Camera Position Increment", &m_xCameraPositionIncrement, m_minCameraPositionIncrement, m_maxCameraPositionIncrement, "%.1f");
  ImGui::SliderFloat("Y Camera Position Increment", &m_yCameraPositionIncrement, m_minCameraPositionIncrement, m_maxCameraPositionIncrement, "%.1f");
  ImGui::SliderFloat("Z Camera Position Increment", &m_zCameraPositionIncrement, m_minCameraPositionIncrement, m_maxCameraPositionIncrement, "%.1f");

  if (ImGui::Button("Reset Camera Position"))
  {
    m_bResetCameraPosition = true;
  }

  if (m_bShowHintsPopup)
  {
    ImGui::OpenPopup("Hints");
    m_bShowHintsPopup = false;
  }

  ShowHintsPopup();

  ImGui::End();

  // Update the configuration with the wire frame color
  m_pConfiguration->render.renderWireframeColorR = static_cast<uint8_t>(m_WireframeColor.x * 255);
  m_pConfiguration->render.renderWireframeColorG = static_cast<uint8_t>(m_WireframeColor.y * 255);
  m_pConfiguration->render.renderWireframeColorB = static_cast<uint8_t>(m_WireframeColor.z * 255);
}

void FPSCameraSettingsGUI::SetModelsList(const std::vector<std::string>& filesList)
{
  m_ModelsList = filesList;
}

float FPSCameraSettingsGUI::GetXCameraPositionIncrement() const
{
  return m_xCameraPositionIncrement;
}

float FPSCameraSettingsGUI::GetYCameraPositionIncrement() const
{
  return m_yCameraPositionIncrement;
}

float FPSCameraSettingsGUI::GetZCameraPositionIncrement() const
{
  return m_zCameraPositionIncrement;
}

bool FPSCameraSettingsGUI::IsCullingEnabled() const
{
  return m_bEnableCulling;
}

bool FPSCameraSettingsGUI::IsResetCameraPosition()
{
  bool reset = m_bResetCameraPosition;
  m_bResetCameraPosition = false; // Reset the flag after checking
  return reset;
}

std::string FPSCameraSettingsGUI::GetSelectedModel()
{
  return m_sSelectedModel;
}

void FPSCameraSettingsGUI::ShowModelsListSelector()
{
  if (ImGui::BeginCombo("Select File", m_ModelsList.empty() ? "Select a file" : m_sSelectedModel.c_str()))
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

void FPSCameraSettingsGUI::ShowHintsPopup()
{
  if (ImGui::BeginPopup("Hints"))
  {
    ImGui::SetWindowSize(ImVec2(500, 500));

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    windowPos.x = center.x - windowSize.x * 0.5f;
    windowPos.y = center.y - windowSize.y * 0.5f;
    ImGui::SetWindowPos(windowPos);

    ImGui::Text("Use Keyboard: A - W - S - D");
    ImGui::Text("Use Keyboard: Up - Down - Left - Right");
    ImGui::Text("Use Keyboard: Click and hold right mouse button for rotation");
    ImGui::Text("Warning: No clipping implemented in this scene, code will crash if model goes out of view");
    if (ImGui::Button("OK"))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void FPSCameraSettingsGUI::SetSelectedModel(const std::string& fileName)
{
  m_sSelectedModel = fileName;
}

void FPSCameraSettingsGUI::SetCullingEnabled(bool enabled)
{
  m_bEnableCulling = enabled;
}
