#include <App.hpp>
#include <cstring>
#include <iostream>

int main(int argc, const char *argv[]) {
  App app;
  app.initialise();

  if (argc == 2)
    if (!strcmp(".m3d", &argv[1][strlen(argv[1]) - 4])) {
      app.model_mgr.load_model(argv[1]);
    }

  app.run();
  app.cleanup();
}
