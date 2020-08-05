#include "TextureMapping.h"
#include "RendererEngine.h"
#include "TextureMappingSettingsGUI.h"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "MathUtil.hpp"

#include "Utilities.h"

#include <vector>
#include <filesystem>

constexpr const char* DIRECTORY = "../../assets/";

TextureMapping::TextureMapping() : m_pModel(nullptr)
{
}

void TextureMapping::Initialize(Configuration* pConfiguration, RendererEngine* pRendererEngine)
{
  Scene::Initialize(pConfiguration, pRendererEngine);
  m_pSceneSettingsGUI = std::make_unique<TextureMappingSettingsGUI>(pConfiguration);
  TextureMappingSettingsGUI* setting = dynamic_cast<TextureMappingSettingsGUI*>(m_pSceneSettingsGUI.get());

  m_pConfiguration->render.meshTranslationZ = 5.0f;
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
  LoadMesh();
}

void TextureMapping::LoadMesh()
{
  // Debugging
  // Debug_LoadMesh();
  // m_pModel->pTexture = std::make_unique<Texture>(m_pConfiguration);
  // m_pModel->pTexture->Load("D:\\Documents\\GitHub\\SoftwareRenderer\\external\\assets\\cube\\BRICK_2A.png");
  // return;

  TextureMappingSettingsGUI* setting = dynamic_cast<TextureMappingSettingsGUI*>(m_pSceneSettingsGUI.get());
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

void TextureMapping::HandleInput(const Uint8* keybaordStates, int mouseX, int mouseY, Uint32 mouseStates, float deltaTime)
{
}

void TextureMapping::Update(float deltaTime)
{
  TextureMappingSettingsGUI* setting = dynamic_cast<TextureMappingSettingsGUI*>(m_pSceneSettingsGUI.get());

  // check if the selected obj file has changed
  if (m_sSelectedModel != setting->GetSelectedModel())
  {
    m_sSelectedModel = setting->GetSelectedModel();
    LoadMesh();
  }

  UpdateMesh(deltaTime);
}

void TextureMapping::Render()
{
  m_pRendererEngine->Render(m_pModel.get());
}

void TextureMapping::Clean()
{
}

void TextureMapping::ProcessEvent(SDL_Event* event, float deltaTime)
{
}

void TextureMapping::UpdateMesh(float deltaTime)
{
  TextureMappingSettingsGUI* setting = dynamic_cast<TextureMappingSettingsGUI*>(m_pSceneSettingsGUI.get());

  Vec3f cameraPosition = { 0, 0, 0 };

  m_pModel->trianglesToRender.clear();

  if (setting->IsResetRotation())
  {
    m_pModel->rotation = { 0, 0, 0 };
  }

  m_pModel->rotation.x += setting->GetXRotationAngleSpeed() * deltaTime;
  m_pModel->rotation.y += setting->GetYRotationAngleSpeed() * deltaTime;
  m_pModel->rotation.z += setting->GetZRotationAngleSpeed() * deltaTime;

  bool isCullingEnabled = setting->IsCullingEnabled();

  m_pModel->translation.z = m_pConfiguration->render.meshTranslationZ;

  Mat4f scaleMatrix = createScaleMatrix(m_pModel->scale);
  Mat4f rotationMatrix = createRotationMatrix(m_pModel->rotation);
  Mat4f translationMatrix = createTranslationMatrix(m_pModel->translation);

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

      // transformedVertex.z += m_pConfiguration->render.meshTranslationZ;
      // transformedVertexCache[j] = createVec3FromVec4(transformedVertex);
      transformedVertexCache[j] = transformedVertex;
    }

    // Back face culling
    Vec3f vectorA = createVec3FromVec4(transformedVertexCache[0]);
    Vec3f vectorB = createVec3FromVec4(transformedVertexCache[1]);
    Vec3f vectorC = createVec3FromVec4(transformedVertexCache[2]);

    Vec3f normal = Cross(vectorB - vectorA, vectorC - vectorA);

    Vec3f cameraRay = cameraPosition - vectorA;

    if (isCullingEnabled && Dot(normal, cameraRay) < 0)
    {
      continue;
    }

    triangle_t projectedTriangle;
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

    if (!m_pModel->faces[i].uvs.empty())
    {
      projectedTriangle.uvs[0] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[0]];
      projectedTriangle.uvs[1] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[1]];
      projectedTriangle.uvs[2] = 1.0f - m_pModel->uvs[m_pModel->faces[i].uvs[2]];
    }

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

void TextureMapping::Debug_LoadMesh()
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
