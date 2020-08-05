#include "SettingsGUI.h"
#include "SceneSettingsGUI.h"

SettingsGUI::SettingsGUI(Configuration* configuration) : m_bShowLogWindow(false), m_pConfiguration(configuration), m_iSceneSelection(0), m_bEnableGridSettings(true), m_bShowWarningPopup(true)
{
  ReloadSettings();
}

void SettingsGUI::Initialize(SDL_Window* window, SDL_Renderer* renderer, std::string scenesList)
{
  m_bShowLogWindow = false;
  m_iSceneSelection = 0;
  m_sScenesList = scenesList;

  ImGui::CreateContext();

  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);
}

void SettingsGUI::ReloadSettings()
{
  m_clearColor.x = m_pConfiguration->render.renderClearColorR / 255.0f;
  m_clearColor.y = m_pConfiguration->render.renderClearColorG / 255.0f;
  m_clearColor.z = m_pConfiguration->render.renderClearColorB / 255.0f;

  m_drawColor.x = m_pConfiguration->render.renderDrawColorR / 255.0f;
  m_drawColor.y = m_pConfiguration->render.renderDrawColorG / 255.0f;
  m_drawColor.z = m_pConfiguration->render.renderDrawColorB / 255.0f;

  m_gridColor.x = m_pConfiguration->render.gridColorR / 255.0f;
  m_gridColor.y = m_pConfiguration->render.gridColorG / 255.0f;
  m_gridColor.z = m_pConfiguration->render.gridColorB / 255.0f;
}

void SettingsGUI::ProcessEvent(SDL_Event* event)
{
  ImGui_ImplSDL2_ProcessEvent(event);
}

void SettingsGUI::Update()
{
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void SettingsGUI::Render(SceneSettingsGUI* sceneGUI)
{
  // Generate the windows to render
  MainSettingsWindow();

  if (m_bShowLogWindow)
  {
    m_pConfiguration->log.Render("Logs");
  }

  if (sceneGUI)
  {
    sceneGUI->Render();
  }

  // Rendering
  ImGui::Render();
  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void SettingsGUI::MainSettingsWindow()
{
  ImGui::Begin("Settings");
  ImGui::Text("Display settings");
  ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImVec2 mousePos = ImGui::GetMousePos();
  // Display the mouse position in the window
  ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);
  ImGui::SliderInt("Framerate Limiter", &m_pConfiguration->display.iFrameRate, 1, 244);
  ImGui::Checkbox("Logs Window", &m_bShowLogWindow);
  ImGui::ColorEdit3("Render Clear", reinterpret_cast<float*>(&m_clearColor));

  m_pConfiguration->render.renderClearColorR = static_cast<uint8_t>(m_clearColor.x * 255);
  m_pConfiguration->render.renderClearColorG = static_cast<uint8_t>(m_clearColor.y * 255);
  m_pConfiguration->render.renderClearColorB = static_cast<uint8_t>(m_clearColor.z * 255);

  ImGui::ColorEdit3("Draw Color", reinterpret_cast<float*>(&m_drawColor));

  m_pConfiguration->render.renderDrawColorR = static_cast<uint8_t>(m_drawColor.x * 255);
  m_pConfiguration->render.renderDrawColorG = static_cast<uint8_t>(m_drawColor.y * 255);
  m_pConfiguration->render.renderDrawColorB = static_cast<uint8_t>(m_drawColor.z * 255);

  ImGui::Checkbox("Enable Grid", &m_bEnableGridSettings);

  m_pConfiguration->render.renderGrid = m_bEnableGridSettings;

  ImGui::BeginDisabled(!m_bEnableGridSettings);

  ImGui::SliderInt("Grid Spacing", &m_pConfiguration->render.gridSpacing, 1, 255);
  ImGui::ColorEdit3("Grid Color", reinterpret_cast<float*>(&m_gridColor));

  m_pConfiguration->render.gridColorR = static_cast<uint8_t>(m_gridColor.x * 255);
  m_pConfiguration->render.gridColorG = static_cast<uint8_t>(m_gridColor.y * 255);
  m_pConfiguration->render.gridColorB = static_cast<uint8_t>(m_gridColor.z * 255);

  ImGui::EndDisabled();

  ImGui::Combo("Scene Selector", &m_iSceneSelection, m_sScenesList.c_str());

  if (m_bShowWarningPopup)
  {
    ImGui::OpenPopup("Hints");
    m_bShowWarningPopup = false;
  }

  ShowWarningPopup();

  ImGui::End();
}

void SettingsGUI::ShowWarningPopup()
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

    ImGui::Text("Warning: No clipping implemented in those scenes, code will crash if model goes out of view");
    if (ImGui::Button("OK"))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void SettingsGUI::Clean()
{
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void SettingsGUI::SetSceneSelection(int sceneIndex)
{
  m_iSceneSelection = sceneIndex;
}

int SettingsGUI::GetSceneSelection()
{
  return m_iSceneSelection;
}

bool SettingsGUI::IsGridSettingsEnabled()
{
  return m_bEnableGridSettings;
}

bool SettingsGUI::IsCapturingMouseKeyboard()
{
  return (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard);
}
