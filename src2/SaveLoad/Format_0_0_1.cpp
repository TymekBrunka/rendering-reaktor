#include "App.hpp"
#include <SaveLoad/Format.hpp>
#include <stdio.h>
#include <zip.h>

bool zip_loader_0_0_1(App *app, zip_t *za, zip_stat_t *stat, zip_error_t *error) {
  char *data = NULL;
  zip_source_t *data_src = NULL;

  if (!strncmp(stat->name, "models/", sizeof("models/") - 1)) {
    fprintf(stderr, "model -> %s\n", stat->name);
    if (!alloc_read_file_from_zip(za, stat->index, &data, data_src, stat)) {
      return false;
    }

    size_t name_length = strlen(stat->name);
    snprintf(formated_path, 1024, "%s%s%s", home_dir, "/reaktory/tmp/models/", stat->name);

    fprintf(stderr, "Saving to file %s\n", formated_path);

    FILE *file = fopen(formated_path, "w");
    fwrite(data, 1, stat->size, file);
    fclose(file);

    app->model_mgr.load_model(std::string{formated_path});

    zip_source_free(data_src);
    delete[] data;
  }

  return true;
}
