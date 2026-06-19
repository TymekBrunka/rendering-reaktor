#include "zipconf.h"
#include <App.hpp>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <zip.h>

#define endzip()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_source_free(src);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  delete[] blob;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_error_fini(&error);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
  return false;

bool alloc_read_file_from_zip(zip_t *za, zip_int64_t idx, char **data, zip_source_t *src, zip_stat_t *stat) {
  src = zip_source_zip_file(za, za, idx, 0, 0, -1, NULL);
  if (src == NULL) {
    fprintf(stderr, "Cannot create source for 'data.txt' from zip archive\n");
    return false;
  }

  zip_stat_t stat_; // local variable to stat to when provided stat is nullptr
  if (stat == nullptr) {
    zip_stat_init(&stat_);
    stat = &stat_;
  }

  if (zip_source_stat(src, stat) < 0) {
    fprintf(stderr, "Can't stat file 'data.txt' in zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    zip_source_free(src);
    return false;
  }

  if (zip_source_open(src) == -1) {
    fprintf(stderr, "Cannot open source for 'data.txt' from zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    zip_source_free(src);
    return false;
  }

  *data = new char[stat->size];
  if (zip_source_read(src, *data, stat->size) <= 0) {
    fprintf(stderr, "Cannot read contents of 'data.txt' from zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    zip_source_free(src);
    delete[] *data;
    return false;
  }

  return true;
}

bool App::import_scene_zip(const char *filepath) {
  FILE *input;
  size_t filesize;
  char *blob;
  // read file to memory
  {
    input = fopen(filepath, "rb");

    if (!input) {
      fprintf(stderr, "Couldn't open file %s\n", filepath);
      return false;
    }

    if (fseek(input, 0, SEEK_END)) {
      fprintf(stderr, "fseek failed on file %s\n", filepath);
      return false;
    }

    filesize = ftell(input);
    fseek(input, 0, SEEK_SET);

    blob = new char[filesize];
    fread(blob, filesize, 1, input);
    fclose(input);
  }

  zip_source_t *src;
  zip_t *za;
  zip_error_t error;
  size_t num_of_entries;

  // open zip archive
  {
    zip_error_init(&error);
    /* create source from buffer */
    if ((src = zip_source_buffer_create(blob, filesize, 1, &error)) == NULL) {
      fprintf(stderr, "Can't create source: %s\n", zip_error_strerror(&error));
      delete[] blob;
      zip_error_fini(&error);
      return false;
    }

    /* open zip archive from source */
    if ((za = zip_open_from_source(src, 0, &error)) == NULL) {
      fprintf(stderr, "Can't open zip from source: %s\n", zip_error_strerror(&error));
      endzip();
    }

    /* if source is empty */
    if (zip_source_is_deleted(src)) {
      fprintf(stderr, "Archive is empty\n");
      endzip();
    }
  }

  zip_int64_t data_txt_idx = zip_name_locate(za, "data.txt", 0);
  if (data_txt_idx == -1) {
    fprintf(stderr, "Zip archive doesnt contain 'data.txt'\n");
    endzip();
  }

  zip_stat_t stat;
  zip_stat_init(&stat);

  {
    char *data_txt = NULL;
    zip_source_t *data_txt_src = NULL;
    if (!alloc_read_file_from_zip(za, data_txt_idx, &data_txt, data_txt_src, &stat)) {
      endzip();
    }

    fprintf(stderr, "Contents of 'data.txt':\n%s\n", data_txt);

    size_t linenr = 0;
    size_t column = 0;

    char *reader_end = &data_txt[stat.size - 1];
    for (char *reader = data_txt; reader <= reader_end; reader++) {
      fprintf(stderr, "reader: %s\n", reader);
      if (*reader == '#')
        while (*reader != '\n' && reader <= reader_end)
          reader++;
    }

    zip_source_free(data_txt_src);
    delete[] data_txt;
  }

  // reading from zip archive
  num_of_entries = zip_get_num_entries(za, 0);
  fprintf(stderr, "-- NUMBER OF ENTRIES: %llu\n", num_of_entries);
  for (size_t i = 0; i < num_of_entries; i++) {
    if (zip_stat_index(za, i, 0, &stat) < 0) {
      fprintf(stderr, "Can't stat file %llu in zip archive : %s\n", i, zip_strerror(za));
      continue;
    }

    fprintf(stderr, "file: %s\n", stat.name);
  }

  zip_source_free(src);
  delete[] blob;
  zip_error_fini(&error);
  return true;
}
