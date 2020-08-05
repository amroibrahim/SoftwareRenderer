#include "SoftwareRenderer.h"

SoftwareRenderer::SoftwareRenderer() :
  m_bIsOver(false),
  m_pSDLWindow(nullptr),
  m_pRendererEngine(nullptr),
  m_pCurrentScene(nullptr),
  m_SettingsGUI(&m_Configuration),
  m_ScenesFactory(&m_Configuration),
  m_iCurrentSceneIndex(0),
  m_lastFrameTime(0),
  m_frameLength(0),
  m_iWindowPositionX(0),
  m_iWindowPositionY(0)
{
}

SoftwareRenderer::~SoftwareRenderer()
{
}

void SoftwareRenderer::Initialize()
{
  m_pSDLWindow = std::make_unique<SDLWindow>();
  m_pSDLWindow->Initialize(&m_Configuration, "Software Renderer");

  std::string startScene("00 - Empty");

  m_iCurrentSceneIndex = m_ScenesFactory.GetSceneIndex(startScene);

  m_SettingsGUI.Initialize(m_pSDLWindow->GetWindows(), m_pSDLWindow->GetRenderer(), m_ScenesFactory.GetSceneList());
  m_SettingsGUI.SetSceneSelection(m_iCurrentSceneIndex);

  m_pRendererEngine = std::make_unique <RendererEngine>(m_pSDLWindow->GetRenderer(), &m_Configuration);

  m_ScenesFactory.CreateScene(m_iCurrentSceneIndex, m_pCurrentScene, m_pRendererEngine.get());
}

void SoftwareRenderer::Run()
{
  while (!IsOver())
  {
    InitializeFrame();
    ProcessInput();
    Update();
    Render();
    Delay();
  }
}

void SoftwareRenderer::ProcessInput()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    m_SettingsGUI.ProcessEvent(&event);

    // Check if ImGui is handling the event
    if (m_SettingsGUI.IsCapturingMouseKeyboard())
    {
      continue;  // Skip processing this event in your main window
    }

    m_pCurrentScene->ProcessEvent(&event, m_deltaTime);

    switch (event.type)
    {
    case SDL_QUIT:
      m_bIsOver = true;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        m_bIsOver = true;
      }
      break;

    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_MOVED)
      {
        m_iWindowPositionX = event.window.data1;
        m_iWindowPositionY = event.window.data2;
      }

    default:
      break;
    }
  }

  // Process key without repeat delays
  int iMouseX = 0;
  int iMouseY = 0;

  Uint32 mouseStates = SDL_GetMouseState(&iMouseX, &iMouseY);
  const Uint8* keybaordStates = SDL_GetKeyboardState(NULL);

  SDL_GetWindowPosition(m_pSDLWindow->GetWindows(), &m_iWindowPositionX, &m_iWindowPositionY);

  int mouseRelativeX = iMouseX - m_iWindowPositionX;
  int mouseRelativeY = iMouseY - m_iWindowPositionY;

  HandleInput(keybaordStates, mouseRelativeX, mouseRelativeY, mouseStates);
}

void SoftwareRenderer::Render()
{
  m_pRendererEngine->ClearSurface(
    m_Configuration.render.renderClearColorR,
    m_Configuration.render.renderClearColorG,
    m_Configuration.render.renderClearColorB);

  // Ask the current scene to render itself
  m_pCurrentScene->Render();

  // Blit the render buffer to the windows renderer
  m_pSDLWindow->BlitBuffer(m_pRendererEngine->GetRenderBuffer());

  // Render the settings GUI on top of the render buffer
  m_SettingsGUI.Render(m_pCurrentScene->m_pSceneSettingsGUI.get());
  m_pSDLWindow->Render();
}

void SoftwareRenderer::Update()
{
  m_SettingsGUI.Update();

  // Check if the scene changed
  SwitchSceneOnSelection();

  m_pCurrentScene->Update(m_deltaTime);
}

void SoftwareRenderer::SwitchSceneOnSelection()
{
  int sceneSelection = m_SettingsGUI.GetSceneSelection();
  if (m_iCurrentSceneIndex != sceneSelection)
  {
    m_iCurrentSceneIndex = sceneSelection;
    m_ScenesFactory.CreateScene(m_iCurrentSceneIndex, m_pCurrentScene, m_pRendererEngine.get());

    // Reload the settings, the scene init might have changed
    // the configuration
    m_SettingsGUI.ReloadSettings();
  }
}

void SoftwareRenderer::HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates)
{
  m_pCurrentScene->HandleInput(keybaordStates, mouseX, mouseY, mouseStates, m_deltaTime);
}

void SoftwareRenderer::Delay()
{
  uint32_t currentFrameLength = SDL_GetTicks() - m_lastFrameTime;
  if (m_frameLength > currentFrameLength)
  {
    SDL_Delay(m_frameLength - currentFrameLength);
  }
}

void SoftwareRenderer::Quit()
{
}

void SoftwareRenderer::Clean()
{
}

bool SoftwareRenderer::IsOver()
{
  return m_bIsOver;
}

void SoftwareRenderer::InitializeFrame()
{
  m_frameLength = 1000 / m_Configuration.display.iFrameRate;
  m_deltaTime = (SDL_GetTicks() - m_lastFrameTime) / 1000.0f;
  m_lastFrameTime = SDL_GetTicks();
}
