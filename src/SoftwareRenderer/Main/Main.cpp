#include <iostream>
#include <memory>

#include "SoftwareRenderer.h"

int main(int argc, char* argv[])
{
  std::unique_ptr <SoftwareRenderer> pSoftwareRenderer = std::make_unique<SoftwareRenderer>();

  pSoftwareRenderer->Initialize();
  pSoftwareRenderer->Run();
  pSoftwareRenderer->Clean();

  return 0;
}
