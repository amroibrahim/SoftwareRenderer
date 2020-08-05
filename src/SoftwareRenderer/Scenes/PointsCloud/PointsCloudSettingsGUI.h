#pragma once

#include "SceneSettingsGUI.h"
#include "Configuration.h"

class PointsCloudSettingsGUI : public SceneSettingsGUI
{
public:
  explicit PointsCloudSettingsGUI(Configuration* configuration);
  void Render() override;

  float GetXRotationAngleSpeed();
  float GetYRotationAngleSpeed();
  float GetZRotationAngleSpeed();

  Configuration* m_pConfiguration;

protected:
  float m_minCameraPosition;
  float m_maxCameraPosition;
  float m_minRotationAngle;
  float m_maxRotationAngle;

  float m_xRotationAngleSpeed;
  float m_yRotationAngleSpeed;
  float m_zRotationAngleSpeed;
};
