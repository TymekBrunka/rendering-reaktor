#include "App.hpp"
#include <SaveLoad/Format.hpp>
#include <stdio.h>
#include <zip.h>

bool loader_0_0_1(SavableState *state, abstract_file *afile) {
  char *data = NULL;
  zip_source_t *data_src = NULL;

  if (!strncmp(afile->path, "models/", sizeof("models/") - 1)) {
    fprintf(stderr, "model -> %s\n", afile->path);

    abstract_memory memory = abstract_file_read(afile);
    abstract_memory_read_data(&memory);
    abstract_file_make_real(afile, &memory);

    state->model_mgr.load_model(std::string{formated_path_2});

    abstract_memory_free(&memory);
  }

  return true;
}

bool scene_loader_0_0_1(SavableState *state, yyjson_val* root) {
  return true;
}
