#pragma once

#include "SceneSettingsGUI.h"
#include "Configuration.h"

#include "Vector3.hpp"
#include "Utilities.h"

class TriangleSettingsGUI : public SceneSettingsGUI
{
public:
  explicit TriangleSettingsGUI(Configuration* configuration);
  void Render() override;

  void SetPoint1(Vec3f point);
  void SetPoint2(Vec3f point);
  void SetPoint3(Vec3f point);

  Vec3f GetPoint1();
  Vec3f GetPoint2();
  Vec3f GetPoint3();

  bool IsFillingEnabled() const;

  Configuration* m_pConfiguration;

  ColorConverter m_ColorConverterPoint1;
  ColorConverter m_ColorConverterPoint2;
  ColorConverter m_ColorConverterPoint3;

protected:
  float m_minCameraPosition;
  float m_maxCameraPosition;
  float m_minRotationAngle;
  float m_maxRotationAngle;

  float m_point1[2];
  float m_point2[2];
  float m_point3[2];

  bool m_bEnableFilling;
  ImVec4 m_Point1Color;
  ImVec4 m_Point2Color;
  ImVec4 m_Point3Color;
};
