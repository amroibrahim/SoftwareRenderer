#include "FPSCamera.h"
#include "RendererEngine.h"
#include "FPSCameraSettingsGUI.h"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "MathUtil.hpp"

#include "Utilities.h"

#include <vector>
#include <filesystem>

constexpr const char* DIRECTORY = "../../assets/";

FPSCamera::FPSCamera() : m_pModel(nullptr), m_iPreviousMouseX(0), m_iPreviousMouseY(0)
{
}

void FPSCamera::Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine)
{
  Scene::Initialize(pConfiguration, pRendererEngine);
  m_pSceneSettingsGUI = std::make_unique<FPSCameraSettingsGUI>(pConfiguration);
  FPSCameraSettingsGUI* setting = dynamic_cast<FPSCameraSettingsGUI*>(m_pSceneSettingsGUI.get());

  m_pConfiguration->render.meshTranslationZ = 10.0f;
  m_pConfiguration->render.renderDrawColorR = 255;
  m_pConfiguration->render.renderDrawColorG = 0;
  m_pConfiguration->render.renderDrawColorB = 0;

  m_Light.direction = { 0, 0, 1 };
  m_Light.direction.Normalize();

  std::vector<std::string> modelsList;
  Utilities::SearchForModelsDirectories(DIRECTORY, modelsList);

  setting->SetModelsList(modelsList);
  setting->SetSelectedModel("cube");
  setting->SetCullingEnabled(true);

  // TODO: add a getter and setter for sorting triangles
  m_pConfiguration->render.renderWireframe = false;
  m_pConfiguration->render.enableTriangleSorting = false;
  m_pConfiguration->render.renderFillTriangles = false;
  m_pConfiguration->render.renderTexturedTriangles = true;

  m_Clipping.Initialize(pConfiguration);

  LoadMesh();
}

void FPSCamera::LoadMesh()
{
  // Debugging
  // Debug_LoadMesh();
  // m_pModel->pTexture = std::make_unique<Texture>(m_pConfiguration);
  // m_pModel->pTexture->Load("D:\\Documents\\GitHub\\SoftwareRenderer\\external\\assets\\cube\\BRICK_2A.png");
  // return;

  FPSCameraSettingsGUI* setting = dynamic_cast<FPSCameraSettingsGUI*>(m_pSceneSettingsGUI.get());
  m_sSelectedModel = setting->GetSelectedModel();

  m_pModel = std::make_unique<Mesh>();
  m_Loader.Load(DIRECTORY + m_sSelectedModel, *m_pModel.get());

  // reset translation position
  m_pConfiguration->render.maxTranslateZ = 100.0f;
  m_pConfiguration->render.meshTranslationZ = 5.0f;

  // Does this model need special handle for the translation?
  if (m_pModel->TranslationZ > m_pConfiguration->render.maxTranslateZ)
  {
    // make the max translation position 1.5 times the camera position
    m_pConfiguration->render.maxTranslateZ = m_pModel->TranslationZ * 1.5f;
    m_pConfiguration->render.meshTranslationZ = m_pModel->TranslationZ;
  }

  if (!m_pModel->textureFileName.empty())
  {
    m_pModel->pTexture = std::make_unique<Texture>(m_pConfiguration);
    m_pModel->pTexture->Load(DIRECTORY + m_sSelectedModel + "/" + m_pModel->textureFileName);
  }
}

void FPSCamera::HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime)
{
  if (keybaordStates[SDL_SCANCODE_RIGHT] || keybaordStates[SDL_SCANCODE_D])
  {
    m_Camera.position.x -= 0.5f * deltaTime;
  }

  if (keybaordStates[SDL_SCANCODE_LEFT] || keybaordStates[SDL_SCANCODE_A])
  {
    m_Camera.position.x += 0.5f * deltaTime;
  }

  // move forward with up arrow or W
  if (keybaordStates[SDL_SCANCODE_UP] || keybaordStates[SDL_SCANCODE_W])
  {
    m_Camera.position.z += 0.5f * deltaTime;
  }

  // move backward with down arrow or S
  if (keybaordStates[SDL_SCANCODE_DOWN] || keybaordStates[SDL_SCANCODE_S])
  {
    m_Camera.position.z -= 0.5f * deltaTime;
  }

  // move up/fly with space
  if (keybaordStates[SDL_SCANCODE_SPACE])
  {
    m_Camera.position.y += 0.5f * deltaTime;
  }

  // move down with left shift/left ctr
  if (keybaordStates[SDL_SCANCODE_LSHIFT] || keybaordStates[SDL_SCANCODE_LCTRL])
  {
    m_Camera.position.y -= 0.5f * deltaTime;
  }

  // rotate the camera with the mouse
  // Calculate the change in mouse position

  // if the mouse is not pressed, reset the previous mouse position
  if (!(mouseStates & SDL_BUTTON(SDL_BUTTON_RIGHT)))
  {
    m_iPreviousMouseX = mouseX;
    m_iPreviousMouseY = mouseY;
  }

  if (mouseStates & SDL_BUTTON(SDL_BUTTON_RIGHT))
  {
    int deltaMouseX = m_iPreviousMouseX - mouseX;
    int deltaMouseY = m_iPreviousMouseY - mouseY;

    m_Camera.yaw += deltaMouseX * 0.01f * deltaTime;
    m_Camera.pitch -= deltaMouseY * 0.01f * deltaTime;

    if (m_Camera.pitch > 89.0f)
    {
      m_Camera.pitch = 89.0f;
    }

    if (m_Camera.pitch < -89.0f)
    {
      m_Camera.pitch = -89.0f;
    }
  }
}

void FPSCamera::Update(float deltaTime)
{
  FPSCameraSettingsGUI* setting = dynamic_cast<FPSCameraSettingsGUI*>(m_pSceneSettingsGUI.get());

  // check if the selected obj file has changed
  if (m_sSelectedModel != setting->GetSelectedModel())
  {
    m_sSelectedModel = setting->GetSelectedModel();
    LoadMesh();
  }

  UpdateMesh(deltaTime);
}

void FPSCamera::Render()
{
  m_pRendererEngine->Render(m_pModel.get());
}

void FPSCamera::Clean()
{
}

void FPSCamera::ProcessEvent(SDL_Event* event, float deltaTime)
{
}

void FPSCamera::UpdateMesh(float deltaTime)
{
  FPSCameraSettingsGUI* setting = dynamic_cast<FPSCameraSettingsGUI*>(m_pSceneSettingsGUI.get());

  m_pModel->trianglesToRender.clear();

  if (setting->IsResetCameraPosition())
  {
    m_Camera.position = { 0, 0, 0 };
    m_Camera.pitch = 0;
    m_Camera.yaw = 0;
    m_Camera.direction = { 0, 0, 1 };
    m_Camera.up = { 0, 1, 0 };
  }

  bool isCullingEnabled = setting->IsCullingEnabled();

  m_pModel->translation.z = m_pConfiguration->render.meshTranslationZ;

  m_Camera.direction = { 0, 0, m_pModel->translation.z };

  m_Camera.position.x += setting->GetXCameraPositionIncrement() * deltaTime;
  m_Camera.position.y += setting->GetYCameraPositionIncrement() * deltaTime;
  m_Camera.position.z += setting->GetZCameraPositionIncrement() * deltaTime;

  // Calculate the view matrix
  Vec3f target = { 0, 0, 1 };
  Mat4f cameraRotationMatrix = createRotationMatrix(m_Camera.pitch, m_Camera.yaw, 0.0f);
  m_Camera.direction = createVec3FromVec4(cameraRotationMatrix * createVec4FromVec3(target));

  target = m_Camera.position + m_Camera.direction;

  Mat4f viewMatrix = createLookAtMatrix(m_Camera.position, target, m_Camera.up);

  Mat4f scaleMatrix = createScaleMatrix(m_pModel->scale);
  Mat4f rotationMatrix = createRotationMatrix(m_pModel->rotation);
  Mat4f translationMatrix = createTranslationMatrix(m_pModel->translation);
// Mat4f lookAtMatrix = createLookAtMatrix(m_Camera.position, m_pModel->translation, { 0, 1, 0 });

  Mat4f projectionMatrix = createPerspectiveMatrix(
                             m_pConfiguration->render.fov,
                             // (float)m_pConfiguration->display.iScreenBufferHeight / (float)m_pConfiguration->display.iScreenBufferWidth,
                             (float)m_pConfiguration->display.iScreenBufferWidth / (float)m_pConfiguration->display.iScreenBufferHeight,
                             m_pConfiguration->render.nearPlane,
                             m_pConfiguration->render.farPlane);

  // Loop through all the faces of the mesh
  for (int i = 0; i < m_pModel->faces.size(); ++i)
  {
    // Get the vertices of the face
    Vec3f faceVertices[3] =
    {
      m_pModel->vertices[m_pModel->faces[i].vertices[0]],
      m_pModel->vertices[m_pModel->faces[i].vertices[1]],
      m_pModel->vertices[m_pModel->faces[i].vertices[2]]
    };

    // Transform the vertices of the face
    Vec4f transformedVertexCache[3];

    for (int j = 0; j < 3; j++)
    {
      Vec4f transformedVertex = createVec4FromVec3(faceVertices[j]);

      // Scale the vertex
      transformedVertex = scaleMatrix * transformedVertex;

      // Rotate the vertex
      transformedVertex = rotationMatrix * transformedVertex;

      // Translate the vertex the z-axis
      transformedVertex = translationMatrix * transformedVertex;

      // apply the view matrix
      transformedVertex = viewMatrix * transformedVertex;

      // transformedVertex.z += m_pConfiguration->render.meshTranslationZ;
      // transformedVertexCache[j] = createVec3FromVec4(transformedVertex);
      transformedVertexCache[j] = transformedVertex;
    }

    // Back face culling
    Vec3f vectorA = createVec3FromVec4(transformedVertexCache[0]);
    Vec3f vectorB = createVec3FromVec4(transformedVertexCache[1]);
    Vec3f vectorC = createVec3FromVec4(transformedVertexCache[2]);

    Vec3f normal = Cross(vectorB - vectorA, vectorC - vectorA);

    Vec3f origin = { 0, 0, 0 };
    Vec3f cameraRay = origin - vectorA;

    if (isCullingEnabled && Dot(normal, cameraRay) < 0)
    {
      continue;
    }

    triangle_t projectedTriangle;

    if (!m_pModel->faces[i].uvs.empty())
    {
      projectedTriangle.uvs[0] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[0]];
      projectedTriangle.uvs[1] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[1]];
      projectedTriangle.uvs[2] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[2]];
    }

    // Clipping
    m_Clipping.ClipTriangle(transformedVertexCache, projectedTriangle.uvs);

    for (int j = 0; j < 3; j++)
    {
      // Vec4f projectedPoint = m_pRendererEngine->ProjectPoint(transformedVertexCache[j]);

      Vec4f projectedPoint = project(projectionMatrix, transformedVertexCache[j]);

      projectedTriangle.vertices[j].x = projectedPoint.x;
      projectedTriangle.vertices[j].y = projectedPoint.y;
      projectedTriangle.vertices[j].z = projectedPoint.z;
      projectedTriangle.vertices[j].w = projectedPoint.w;

      projectedTriangle.vertices[j].x = projectedTriangle.vertices[j].x * (m_pConfiguration->display.iScreenBufferWidth / 2.0f);
      projectedTriangle.vertices[j].y = projectedTriangle.vertices[j].y * (m_pConfiguration->display.iScreenBufferHeight / 2.0f);

      projectedTriangle.vertices[j].x += (m_pConfiguration->display.iScreenBufferWidth / 2.0f);
      projectedTriangle.vertices[j].y += (m_pConfiguration->display.iScreenBufferHeight / 2.0f);
    }

    if (m_pConfiguration->render.enableTriangleSorting)
    {
      projectedTriangle.depth = (transformedVertexCache[0].z + transformedVertexCache[1].z + transformedVertexCache[2].z) / 3.0f;
    }


    // calculate the light intensity based on the normal of the face
    normal.Normalize();

    float lightIntensity = -Dot(normal, m_Light.direction);

    // clamp the light intensity
    lightIntensity = std::clamp(lightIntensity, 0.0f, 1.0f);

    ColorConverter colorRenderConverter{};
    colorRenderConverter.r = (uint8_t)(m_pConfiguration->render.renderDrawColorR * lightIntensity);
    colorRenderConverter.g = (uint8_t)(m_pConfiguration->render.renderDrawColorG * lightIntensity);
    colorRenderConverter.b = (uint8_t)(m_pConfiguration->render.renderDrawColorB * lightIntensity);
    colorRenderConverter.a = 255;

    projectedTriangle.colors[0] = colorRenderConverter.color;
    projectedTriangle.colors[1] = colorRenderConverter.color;
    projectedTriangle.colors[2] = colorRenderConverter.color;



    m_pModel->trianglesToRender.push_back(projectedTriangle);
  }

  // Sort the triangles to render
  if (m_pConfiguration->render.enableTriangleSorting)
  {
    std::sort(m_pModel->trianglesToRender.begin(), m_pModel->trianglesToRender.end(), [](const triangle_t& a, const triangle_t& b)
    {
      return a.depth > b.depth;
    });
  }
}

void FPSCamera::Debug_LoadMesh()
{
  m_pModel = std::make_unique<Mesh>();

  m_pModel->vertices =
  {
    { -1, -1, -1 }, // 0
    { -1,  1, -1 }, // 1
    {  1,  1, -1 }, // 2
    {  1, -1, -1 }, // 3
    {  1,  1,  1 }, // 4
    {  1, -1,  1 }, // 5
    { -1,  1,  1 }, // 6
    { -1, -1,  1 }  // 7
  };

  m_pModel->uvs =
  {
    { 0, 0 }, // 0
    { 0, 1 }, // 1
    { 1, 1 }, // 2
    { 1, 0 }  // 3
  };

  std::vector<face_t> face_vector =
  {
    { { 0, 1, 2 }, { 0, 1, 2 } },
    { { 0, 2, 3 }, { 0, 2, 3 } },
    { { 3, 2, 4 }, { 0, 1, 2 } },
    { { 3, 4, 5 }, { 0, 2, 3 } },
    { { 5, 4, 6 }, { 0, 1, 2 } },
    { { 5, 6, 7 }, { 0, 2, 3 } },
    { { 7, 6, 1 }, { 0, 1, 2 } },
    { { 7, 1, 0 }, { 0, 2, 3 } },
    { { 1, 6, 4 }, { 0, 1, 2 } },
    { { 1, 4, 2 }, { 0, 2, 3 } },
    { { 5, 7, 0 }, { 0, 1, 2 } },
    { { 5, 0, 3 }, { 0, 2, 3 } }
  };

  m_pModel->faces = face_vector;
}
