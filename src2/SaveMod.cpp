#include <App.hpp>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <zip.h>

bool App::import_scene_zip(const char *filepath) {
  FILE *input = fopen(filepath, "r");

  if (!input)
    fprintf(stderr, "Couldn't open file %s\n", filepath);
  return false;

  if (fseek(input, 0, SEEK_END)) {
    fprintf(stderr, "fseek failed on file %s\n", filepath);
    return false;
  }

  size_t bytes_read = ftell(input);
  fseek(input, 0, SEEK_SET);

  char *blob = new char[bytes_read];
  fread(blob, bytes_read, 1, input);
  fclose(input);
}
