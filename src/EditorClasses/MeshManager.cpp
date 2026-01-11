#include "MeshManager.hpp"

#include "tiny_obj_loader.h"

// #include <nfd.h>
// #include <stdio.h>
// #include <stdlib.h>
#include "portable-file-dialogs.h"

namespace MeshManager {

void load_from_file() {
  // clang-format off
  pfd::open_file f = pfd::open_file("Wybierz plik z modelem 3D", pfd::path::home(),
                          { "Modele 3D (.obj)", "*.obj",
                            "Wszystkie pliki", "*" },
                          pfd::opt::none);
  // clang-format on
  // std::cout << "Selected files:";
  // for (auto const &name : f.result())
  //   std::cout << " " + name;
  // std::cout << "\n";
  
}

} // namespace MeshManager
