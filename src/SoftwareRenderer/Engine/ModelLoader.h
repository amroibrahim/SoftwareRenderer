#pragma once

#include <functional>
#include <string>
#include <sstream>
#include <map>

#include "Mesh.h"

class ModelLoader
{
public:
  ModelLoader();
  bool Load(const std::string& sModelDir, Mesh& model);

protected:
  void Split(const std::string& str, char delimiter, std::vector<std::string>& tokensOut);

  void PareseFace(std::istringstream& iss, Mesh& model);
  void PareseVertex(std::istringstream& iss, Mesh& model);
  void PareseVertexNormals(std::istringstream& iss, Mesh& model);
  void PareseVertexTexture(std::istringstream& iss, Mesh& model);

  void PareseObjFileName(std::istringstream& iss, Mesh& model);
  void PareseTextureFileName(std::istringstream& iss, Mesh& model);
  void PareseCameraDistance(std::istringstream& iss, Mesh& model);

  std::map<std::string, std::function<void(std::istringstream&, Mesh&)>> m_ObjParserLookup;
  std::map<std::string, std::function<void(std::istringstream&, Mesh&)>> m_SettingsParserLookup;

private:
  bool LoadObj(const std::string& sObjFileName, Mesh& model);
  void LoadSettings(const std::string sSettingsFile, Mesh& model);
};

