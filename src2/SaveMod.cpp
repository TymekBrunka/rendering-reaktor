#include "zipconf.h"
#include <App.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <zip.h>

#define SCOPE(__VA_ARGS__)

#define endzip()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_source_free(src);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  delete[] blob;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_error_fini(&error);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
  return false;

#define if_field_is(x)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
  if (reader + sizeof((x)) - 1 <= reader_end + 1)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (!strncmp(reader, (x), sizeof((x)) - 1))

static bool alloc_read_file_from_zip(zip_t *za, zip_int64_t idx, char **data, zip_source_t *src, zip_stat_t *stat) {
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

static bool parse_version(const char *reader, size_t line_length, int *major, int *minor, int *patch) {
  if (line_length < 5) {
    fprintf(stderr, "Wrong version format: version shorter than 5 characters");
    return false;
  }

  int *num = major;
  while(int i = 0; i < line_length; i++) {
    if (*reader >= '0' && *reader <= '9') {
      *num = (num * 10) + (*reader - '0');
    } else if (*reader == '.') {

    }
  }
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

  SCOPE("open zip archive") {

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

  zip_stat_t stat;
  zip_stat_init(&stat);

  SCOPE("reading contents of data.txt") {

    zip_int64_t data_txt_idx = zip_name_locate(za, "data.txt", 0);
    if (data_txt_idx == -1) {
      fprintf(stderr, "Zip archive doesnt contain 'data.txt'\n");
      endzip();
    }

    char *data_txt = NULL;
    zip_source_t *data_txt_src = NULL;
    if (!alloc_read_file_from_zip(za, data_txt_idx, &data_txt, data_txt_src, &stat)) {
      endzip();
    }

    fprintf(stderr, "Contents of 'data.txt':\n%s\n", data_txt);

    size_t linenr = 1;
    size_t column = 0;

    char *reader_end = &data_txt[stat.size - 1];
    for (char *reader = data_txt; reader <= reader_end;) {
      // fprintf(stderr, "reader: %s\n", reader);

      if (*reader == '#') {
        while (*reader != '\n' && reader <= reader_end)
          reader++;
        linenr++;
        // column = 0;
      }

      size_t line_length = 0;
      for (char *c = reader; c <= reader_end && *c != '\n'; c++)
        line_length++;

      if_field_is("app version: ") {
        reader += sizeof("app version: ") - 1;
        int major, minor, patch;
        if (!parse_version(reader, line_length - sizeof("app version: ") + 1, &major, &minor, &patch)) {
        }
      }
    }

    zip_source_free(data_txt_src);
    delete[] data_txt;
  }

  SCOPE("reading from zip archive") {

    num_of_entries = zip_get_num_entries(za, 0);
    fprintf(stderr, "-- NUMBER OF ENTRIES: %llu\n", num_of_entries);
    for (size_t i = 0; i < num_of_entries; i++) {
      if (zip_stat_index(za, i, 0, &stat) < 0) {
        fprintf(stderr, "Can't stat file %llu in zip archive : %s\n", i, zip_strerror(za));
        continue;
      }

      fprintf(stderr, "file: %s\n", stat.name);
    }
  }

  zip_source_free(src);
  delete[] blob;
  zip_error_fini(&error);
  return true;
}
