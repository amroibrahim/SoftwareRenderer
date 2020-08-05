#pragma once

#include "SceneSettingsGUI.h"
#include "Configuration.h"

#include <vector>
#include <string>

class TextureMappingSettingsGUI : public SceneSettingsGUI
{
public:
  explicit TextureMappingSettingsGUI(Configuration* configuration);

  void Render() override;
  void SetModelsList(const std::vector<std::string>& filesList);
  void SetSelectedModel(const std::string& fileName);
  void SetCullingEnabled(bool enabled);

  float GetXRotationAngleSpeed() const;
  float GetYRotationAngleSpeed() const;
  float GetZRotationAngleSpeed() const;

  bool IsCullingEnabled() const;
  bool IsResetRotation();

  std::string GetSelectedModel();

  Configuration* m_pConfiguration;

protected:
  void ShowModelsListSelector();

  ImVec4 m_WireframeColor;

  float m_minTranslateZ;
  float m_maxTranslatePosition;
  float m_minRotationAngle;
  float m_maxRotationAngle;

  float m_xRotationAngleSpeed;
  float m_yRotationAngleSpeed;
  float m_zRotationAngleSpeed;

  bool m_bEnableCulling;
  bool m_bEnableFillTriangles;
  bool m_bEnableWireframe;
  bool m_bResetRotation;

  std::vector<std::string> m_ModelsList;
  std::string m_sSelectedModel;
};
