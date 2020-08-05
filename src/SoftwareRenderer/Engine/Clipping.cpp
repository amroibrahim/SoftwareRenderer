#include "Clipping.h"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "MathUtil.hpp"

Clipping::Clipping() : m_pConfiguration(nullptr)
{
}

Clipping::~Clipping()
{
}

void Clipping::Initialize(Configuration* pConfiguration)
{
  m_pConfiguration = pConfiguration;
  InitFrustumClippingPlanes();
}

void Clipping::ClipTriangle(Vec4f* transformedVertexCache, Vec2f* UVs)
{
  polygon_t polygon;

  // polygon always starts as triangle (3 vertices)

  polygon.numVertices = 3;

  for (int i = 0; i < 3; ++i)
  {
    polygon.vertices[i] = transformedVertexCache[i];
    polygon.uvs[i] = UVs[i];
  }

  for (int i = 0; i < frustum_plane_t::NUM_FRUSTUM_PLANES; ++i)
  {
    ClipPolygonAgainstPlane(polygon, m_FrustumPlanes[i]);
  }
}

void Clipping::InitFrustumClippingPlanes()
{
  float cosHalfFOV = cos(m_pConfiguration->render.fov / 2.0f);
  float sinHalfFOV = sin(m_pConfiguration->render.fov / 2.0f);

  m_FrustumPlanes[LEFT_FRUSTUM_PLANE].vertex = { 0, 0, 0 };
  m_FrustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cosHalfFOV;
  m_FrustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
  m_FrustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sinHalfFOV;

  m_FrustumPlanes[RIGHT_FRUSTUM_PLANE].vertex = { 0, 0, 0 };
  m_FrustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cosHalfFOV;
  m_FrustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
  m_FrustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sinHalfFOV;

  m_FrustumPlanes[TOP_FRUSTUM_PLANE].vertex = { 0, 0, 0 };
  m_FrustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
  m_FrustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cosHalfFOV;
  m_FrustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sinHalfFOV;

  m_FrustumPlanes[BOTTOM_FRUSTUM_PLANE].vertex = { 0, 0, 0 };
  m_FrustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
  m_FrustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cosHalfFOV;
  m_FrustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sinHalfFOV;

  m_FrustumPlanes[NEAR_FRUSTUM_PLANE].vertex = { 0, 0, m_pConfiguration->render.nearPlane };
  m_FrustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
  m_FrustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
  m_FrustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

  m_FrustumPlanes[FAR_FRUSTUM_PLANE].vertex = { 0, 0, m_pConfiguration->render.farPlane };
  m_FrustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
  m_FrustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
  m_FrustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

void Clipping::ClipPolygonAgainstPlane(polygon_t& polygon, plane_t& frustumPlane)
{
  Vec3f& planeVertex = frustumPlane.vertex;
  Vec3f& planeNormal = frustumPlane.normal;

  polygon_t insideVertices;

  Vec4f* currentVertex = &polygon.vertices[0];
  Vec4f* previousVertex = &polygon.vertices[polygon.numVertices - 1];

  // float currentDot = Dot(*currentVertex - planePoint, planeNormal);
  float previousDot = Dot(*previousVertex - planeVertex, planeNormal);

  for (int i = 0; i < polygon.numVertices; ++i)
  {
    float currentDot = Dot(*currentVertex - planeVertex, planeNormal);

    if (currentDot * previousDot < 0.0f)
    {
      // intersection
      float t = previousDot / (previousDot - currentDot);

      insideVertices.vertices[insideVertices.numVertices] = *previousVertex + (*currentVertex - *previousVertex) * t;

      int uvPreviousIndex = (i == 0) ? polygon.numVertices - 1 : i - 1;
      insideVertices.uvs[insideVertices.numVertices] = polygon.uvs[uvPreviousIndex] + (polygon.uvs[i] - polygon.uvs[uvPreviousIndex]) * t;
      insideVertices.numVertices++;
    }

    if (currentDot > 0.0f)
    {
      insideVertices.vertices[insideVertices.numVertices] = *currentVertex;
      insideVertices.numVertices++;
    }

    previousDot = currentDot;
    previousVertex = currentVertex;
    currentVertex = &polygon.vertices[(i + 1) % polygon.numVertices];
  }

  // copy insideVertices to polygon
  polygon = insideVertices;

 }
