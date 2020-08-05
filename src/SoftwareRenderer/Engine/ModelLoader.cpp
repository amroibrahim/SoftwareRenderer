#include "ModelLoader.h"
#include "Utilities.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

#include "Vector3.hpp"

using namespace std;

ModelLoader::ModelLoader()
{
  m_ObjParserLookup["v"] = std::bind(&ModelLoader::PareseVertex, this, std::placeholders::_1, std::placeholders::_2);
  m_ObjParserLookup["f"] = std::bind(&ModelLoader::PareseFace, this, std::placeholders::_1, std::placeholders::_2);
  m_ObjParserLookup["vn"] = std::bind(&ModelLoader::PareseVertexNormals, this, std::placeholders::_1, std::placeholders::_2);
  m_ObjParserLookup["vt"] = std::bind(&ModelLoader::PareseVertexTexture, this, std::placeholders::_1, std::placeholders::_2);

  m_SettingsParserLookup["obj"] = std::bind(&ModelLoader::PareseObjFileName, this, std::placeholders::_1, std::placeholders::_2);
  m_SettingsParserLookup["tex"] = std::bind(&ModelLoader::PareseTextureFileName, this, std::placeholders::_1, std::placeholders::_2);
  m_SettingsParserLookup["dst"] = std::bind(&ModelLoader::PareseCameraDistance, this, std::placeholders::_1, std::placeholders::_2);
}

bool ModelLoader::Load(const string& sModelDir, Mesh& model)
{
  // Check if settings file exist
  string sSettingsFile = sModelDir + "/settings.txt";
  if (std::filesystem::exists(sSettingsFile))
  {
    LoadSettings(sSettingsFile, model);
    LoadObj(sModelDir + "/" + model.objFileName, model);
  }
  else
  {
    // look for obj file to load
    vector<string> objFiles = Utilities::SearchForObjFiles(sModelDir);

    // any obj file found?
    if (!objFiles.empty())
    {
      // just load the first obj found
      model.objFileName = objFiles[0];
      LoadObj(objFiles[0], model);
    }
  }

  return true;
}

bool ModelLoader::LoadObj(const string& sObjFileName, Mesh& model)
{
  std::ifstream pFile;
  pFile.open(sObjFileName, ifstream::in);
  if (pFile.fail())
  {
    return false;
  }

  cout << "Loading Obj Model: " << sObjFileName << endl;

  string sLine;
  string sLinePrefix;


  while (!pFile.eof())
  {
    getline(pFile, sLine);

    // Skip empty lines
    if (sLine.empty())
    {
      continue;
    }

    istringstream iss(sLine);
    iss >> sLinePrefix;

    // Check if the line prefix is supported
    if (m_ObjParserLookup.count(sLinePrefix))
    {
      // handle the line
      m_ObjParserLookup[sLinePrefix](iss, model);
    }
  }
  return true;
}

void ModelLoader::PareseFace(istringstream& iss, Mesh& model)
{
  face_t face;
  std::string token;

  while (iss >> token)
  {
    std::vector<std::string> vertexData;
    Split(token, '/', vertexData);

    // subtract 1 because obj file indices are 1-based
    if(vertexData.size() > 0 && !vertexData[0].empty())
    {
      face.vertices.push_back(std::stoi(vertexData[0]) - 1);
    }

    if (vertexData.size() > 1 && !vertexData[1].empty())
    {
      face.uvs.push_back(std::stoi(vertexData[1]) - 1);
    }

    if (vertexData.size() > 2 && !vertexData[2].empty())
    {
      face.normals.push_back(std::stoi(vertexData[2]) - 1);
    }
  }

  if (face.vertices.size() != 3)
  {
    cout << "Invalid vertex data: " << iss.str() << endl;
    return;
  }

  model.faces.push_back(face);
}

void ModelLoader::PareseVertex(istringstream& iss, Mesh& model)
{
  Vec3f vec;
  iss >> vec.x >> vec.y >> vec.z;
  model.vertices.push_back(vec);
}

void ModelLoader::PareseVertexNormals(std::istringstream& iss, Mesh& model)
{
  Vec3f vec;
  iss >> vec.x >> vec.y >> vec.z;
  model.normals.push_back(vec);
}

void ModelLoader::PareseVertexTexture(std::istringstream& iss, Mesh& model)
{
  Vec2f vec;
  iss >> vec.x >> vec.y;
  model.uvs.push_back(vec);
}

void ModelLoader::PareseObjFileName(std::istringstream& iss, Mesh& model)
{
  iss >> model.objFileName;
}

void ModelLoader::PareseTextureFileName(std::istringstream& iss, Mesh& model)
{
  iss >> model.textureFileName;
}

void ModelLoader::PareseCameraDistance(std::istringstream& iss, Mesh& model)
{
  iss >> model.TranslationZ;
}

void ModelLoader::LoadSettings(const string sSettingsFile, Mesh& model)
{
  std::ifstream pFile;
  pFile.open(sSettingsFile, ifstream::in);
  if (pFile.fail())
  {
    std::cout << "Error: failed to load settings file " << sSettingsFile << std::endl;
    return;
  }

  cout << "Loading model settings: " << sSettingsFile << endl;

  string sLine;
  string sLinePrefix;

  while (!pFile.eof())
  {
    getline(pFile, sLine);

    // Skip empty lines
    if (sLine.empty())
    {
      continue;
    }

    istringstream iss(sLine);
    iss >> sLinePrefix;

    // Check if the line prefix is supported
    if (m_SettingsParserLookup.count(sLinePrefix))
    {
      // handle the line
      m_SettingsParserLookup[sLinePrefix](iss, model);
    }
  }

}

void ModelLoader::Split(const std::string& str, char delimiter, std::vector<std::string>& tokensOut)
{
  std::string token;
  std::stringstream ss(str);

  while (std::getline(ss, token, delimiter))
  {
    tokensOut.push_back(token);
  }
}