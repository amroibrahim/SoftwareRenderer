#pragma once

#include <vector>
#include <string>
#include <filesystem>

union ColorConverter
{
  uint32_t color;
  struct
  {
    uint8_t r, g, b, a;
  };
};

class Utilities
{
public:
  static void SearchForModelsDirectories(const std::string& directory, std::vector<std::string>& out);
  static std::vector<std::string> SearchForObjFiles(const std::string& modelDir);
};
