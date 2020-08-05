#pragma once
#include "MeshPrimitives.h"
#include "Configuration.h"

class Clipping
{
public:
  Clipping();
  ~Clipping();

  void Initialize(Configuration* pConfiguration);
  void ClipTriangle(Vec4f* transformedVertexCache, Vec2f* UVs);
protected:

  void InitFrustumClippingPlanes();

  plane_t m_FrustumPlanes[frustum_plane_t::NUM_FRUSTUM_PLANES];
  Configuration* m_pConfiguration;
private:
  void ClipPolygonAgainstPlane(polygon_t& polygon, plane_t& frustumPlane);
};

