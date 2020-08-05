#include "Utilities.h"

void Utilities::SearchForModelsDirectories(const std::string& directory, std::vector<std::string>& out)
{
  for (const auto& entry : std::filesystem::directory_iterator(directory))
  {
    //if (entry.is_regular_file())
    //{
    //  // Only add files, not directories
    //  out.push_back(entry.path().filename().string());
    //}
    //else
    if (entry.is_directory())
    {
      out.push_back(entry.path().filename().string());
      // Iterate over the subdirectory
      //for (const auto& subEntry : std::filesystem::directory_iterator(entry.path()))
      //{
      //  if (subEntry.is_regular_file() && subEntry.path().extension() == ".obj")
      //  {
      //    // Add .obj files found in the subdirectory
      //    out.push_back(entry.path().filename().string() + "/" + subEntry.path().filename().string());
      //  }
      //}
    }
  }
}

std::vector<std::string> Utilities::SearchForObjFiles(const std::string& modelDir)
{
  std::vector<std::string> out;
  //Iterate over the subdirectory
  for (const auto& subEntry : std::filesystem::directory_iterator(modelDir))
  {
    if (subEntry.is_regular_file() && subEntry.path().extension() == ".obj")
    {
      // Add .obj files found in the subdirectory
      out.push_back(modelDir + "/" + subEntry.path().filename().string());
    }
  }
  return out;
}
