#include <App.hpp>
#include <cstring>
#include <iostream>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  App app;
  if (!app.initialise())
    return 1;

  if (argc == 2)
    if (!app.import_scene_zip(argv[1])) {
      std::cerr << "Failed to load the scene\n";
    }

  app.run();
  app.cleanup();
}
