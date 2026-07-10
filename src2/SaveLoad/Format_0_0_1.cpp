#include "App.hpp"
#include <SaveLoad/Format.hpp>
#include <stdio.h>
#include <zip.h>

bool zip_loader_0_0_1(App *app, abstract_file *afile) {
  char *data = NULL;
  zip_source_t *data_src = NULL;

  if (!strncmp(afile->path, "models/", sizeof("models/") - 1)) {
    fprintf(stderr, "model -> %s\n", afile->path);
    // if (!alloc_read_file_from_zip(za, stat->index, &data, data_src, stat)) {
    //   return false;
    // }

    abstract_memory memory = abstract_file_read(afile);
    abstract_memory_read_data(&memory);

    // size_t name_length = strlen(stat->name);
    snprintf(formated_path, 1024, "%s%s%s", cwd_path, MODELS_DIR, &afile->path[sizeof("models/") - 1]);

    fprintf(stderr, "Saving to file %s\n", formated_path);

    FILE *file = fopen(formated_path, "wb");
    if (file == NULL) {
      fprintf(stderr, "Couldn't open file %s for writing; exited with ferror = %d\n", formated_path, ferror(file));
      return false;
    }

    fwrite(memory.data, 1, memory.size, file);
    fclose(file);

    app->model_mgr.load_model(std::string{formated_path});

    abstract_memory_free(&memory);
  }

  return true;
}
