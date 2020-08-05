#pragma once

#include "SceneSettingsGUI.h"
#include "Configuration.h"

#include <vector>
#include <string>

class FPSCameraSettingsGUI : public SceneSettingsGUI
{
public:
  explicit FPSCameraSettingsGUI(Configuration* configuration);

  void Render() override;
  void SetModelsList(const std::vector<std::string>& filesList);
  void SetSelectedModel(const std::string& fileName);
  void SetCullingEnabled(bool enabled);

  float GetXCameraPositionIncrement() const;
  float GetYCameraPositionIncrement() const;
  float GetZCameraPositionIncrement() const;

  bool IsCullingEnabled() const;
  bool IsResetCameraPosition();

  std::string GetSelectedModel();

  Configuration* m_pConfiguration;

protected:
  void ShowModelsListSelector();

  ImVec4 m_WireframeColor;

  float m_minTranslateZ;
  float m_maxTranslatePosition;
  float m_minCameraPositionIncrement;
  float m_maxCameraPositionIncrement;

  float m_xCameraPositionIncrement;
  float m_yCameraPositionIncrement;
  float m_zCameraPositionIncrement;

  bool m_bEnableCulling;
  bool m_bEnableFillTriangles;
  bool m_bEnableWireframe;
  bool m_bResetCameraPosition;

  std::vector<std::string> m_ModelsList;
  std::string m_sSelectedModel;
private:
  void ShowHintsPopup();
  bool m_bShowHintsPopup;
};
