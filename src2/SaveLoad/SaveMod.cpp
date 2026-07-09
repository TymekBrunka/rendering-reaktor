#include "zipconf.h"
#include <App.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <zip.h>

#include <dirent.h>
#include <errno.h>
#include <filesystem>

#include <first_launch_data.h>

char *home_dir = NULL;
char cwd_path[1024] = {0};
char formated_path[1024] = {0};

#include "Format_0_0_1.cpp"

#define SCOPE(__VA_ARGS__)

#define endzip()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_source_free(src);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  delete[] blob;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_error_fini(&error);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
  return false;

#define if_field_is(x)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
  if (reader + sizeof((x)) - 1 <= reader_end + 1)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (!strncmp(reader, (x), sizeof((x)) - 1))

bool mkdirIfNotExists(const char *x) {
  snprintf(formated_path, 1024, "%s/%s", cwd_path, (x));
  DIR *dir = opendir(formated_path);
  if (dir)
    closedir(dir);
  else if (errno == ENOENT) {
    std::filesystem::create_directory(formated_path);
    return false;
  }
  return true;
}

bool alloc_read_file_from_zip(zip_t *za, zip_int64_t idx, char **data, zip_source_t *src, zip_stat_t *stat) {
  src = zip_source_zip_file(za, za, idx, 0, 0, -1, NULL);
  if (src == NULL) {
    fprintf(stderr, "Cannot create source for 'data.txt' from zip archive\n");
    return false;
  }

  zip_stat_t stat_; // local variable to stat to when provided stat is NULL
  if (stat == NULL) {
    zip_stat_init(&stat_);
    stat = &stat_;
  }

  if (stat->name == NULL)
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

  *data = new char[stat->size + 1];
  if (zip_source_read(src, *data, stat->size) <= 0) {
    fprintf(stderr, "Cannot read contents of 'data.txt' from zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    zip_source_free(src);
    delete[] *data;
    return false;
  }
  (*data)[stat->size] = '\0';

  return true;
}

static bool parse_version(const char *reader, size_t line_length, int *major, int *minor, int *patch) {
  // if (line_length < 5) {
  //   fprintf(stderr, "Wrong version format: version shorter than 5 characters\n");
  //   return false;
  // }
  *major = 0;
  *minor = 0;
  *patch = 0;
  int *tab[3] = {major, minor, patch};
  int **num = tab;
  for (size_t i = 0; i < line_length; i++, reader++) {
    if (*reader >= '0' && *reader <= '9') {
      **num = (**num * 10) + (*reader - '0');
    } else if (*reader == '.') {
      if (*num != patch)
        num++;
      else {
        fprintf(stderr, "Wrong version format: more than 2 dots\n");
        return false;
      }
    }
  }

  return true;
}

bool App::read_data_txt(char* data_txt, size_t data_txt_len, AppMetadata* meta) {
  bool possibly_had_lost_its_files = false;

  fprintf(stderr, "Contents of 'data.txt':\n>>>>\n%s\n<<<<\n", data_txt);

  // size_t linenr = 1;
  // size_t column = 0;

  char *reader_end = &data_txt[data_txt_len - 1];
  for (char *reader = data_txt; reader <= reader_end;) {
    // fprintf(stderr, "reader: %s\n", reader);

    if (*reader != '#') {
      size_t line_length = 0;
      for (char *c = reader; c <= reader_end && *c != '\n'; c++)
        line_length++;

      if_field_is("app version: ") {
        reader += sizeof("app version: ") - 1;
        if (!parse_version(reader, line_length - sizeof("app version: ") + 1, &meta->app_major, &meta->app_minor, &meta->app_patch))
          return false;
        fprintf(stderr, "App Version is: %d.%d.%d\n", meta->app_major, meta->app_minor, meta->app_patch);
      }

      if_field_is("format version: ") {
        reader += sizeof("format version: ") - 1;
        if (!parse_version(reader, line_length - sizeof("format version: ") + 1, &meta->format_major, &meta->format_minor, &meta->format_patch))
          return false;
        fprintf(stderr, "Format Version is: %d.%d.%d\n", meta->format_major, meta->format_minor, meta->format_patch);
      }

      if_field_is("possibly had lost its files: ") {
        if (line_length >= sizeof("possibly had lost its files: ")) {
          reader += sizeof("possibly had lost its files: ") - 1;
          if (*reader == 'y')
            meta->possibly_had_lost_its_files = true;
          else if (*reader != 'n') {
            fprintf(stderr, "Wrong format: field 'possibly had lost its files must start with either y or n\n");
            return false;
          }
          fprintf(stderr, "Had possibly lost its files: %d\n", meta->possibly_had_lost_its_files);
        }
      }
    }

    while (*reader != '\n' && reader <= reader_end)
      reader++;
    if (*reader == '\n') // so it doesnt trap on newline
      reader++;
    // linenr++;
  }

  return true;
}

bool App::import_scene_zip(const char *filepath) {
  FILE *input;
  size_t filesize;
  char *blob;

  SCOPE("read file to memory") {

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

  AppMetadata meta;
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
    
    if (!read_data_txt(data_txt, stat.size, &meta)) {
      fprintf(stderr, "i need free\n");
      zip_source_free(data_txt_src);
      delete[] data_txt;
      endzip();
    }

    zip_source_free(data_txt_src);
    delete[] data_txt;
  }

  bool (*loader)(App *, zip_t *, zip_stat_t *, zip_error_t *) = NULL;

  SCOPE("selecting proper loader") {

    if (meta.format_major == 0) {
      if (meta.format_minor == 0 && meta.format_patch == 1) {
        loader = zip_loader_0_0_1;
      }
    }

    if (loader == NULL) {
      fprintf(stderr, "Couldnt find loader for format version %d.%d.%d\n", meta.format_major, meta.format_minor, meta.format_patch);
      endzip();
    }
  }

  SCOPE("reading from zip archive") {

    snprintf(cwd_path, 1024, "%s" ROOTDIR TMPDIR, home_dir);
    num_of_entries = zip_get_num_entries(za, 0);
    fprintf(stderr, "-- NUMBER OF ENTRIES: %llu\n", num_of_entries);
    for (size_t i = 0; i < num_of_entries; i++) {
      if (zip_stat_index(za, i, 0, &stat) < 0) {
        fprintf(stderr, "Can't stat file %llu in zip archive : %s\n", i, zip_strerror(za));
        continue;
      }

      if (!strcmp(stat.name, "data.txt"))
        continue;

      fprintf(stderr, "file: %s\n", stat.name);

      if (!loader(this, za, &stat, &error)) {
        endzip();
      }
    }
  }

  metadata = meta;

  zip_source_free(src);
  delete[] blob;
  zip_error_fini(&error);
  return true;
}

#define makeFileIfNotExists(x, contents, size)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
  snprintf(formated_path, 1024, "%s" SEP x, cwd_path);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
  if ((tmpf = fopen(formated_path, "rb")) == NULL) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  \
    if ((tmpf = fopen(formated_path, "wb")) != NULL) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
      fwrite((contents), 1, (size), tmpf);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
      fclose(tmpf);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
    } else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          \
      fprintf(stderr, "Failed to open %s for write.\n", formated_path);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
      return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 \
  } else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    fclose(tmpf);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
  }

bool App::load_app() {
  SCOPE("handle first launch / missing files") {

    snprintf(cwd_path, 1024, "%s" SEP "%s", home_dir, ".reaktory");
    bool is_first_time = false;

    is_first_time = mkdirIfNotExists("");
    mkdirIfNotExists("/models/");
    mkdirIfNotExists("/tmp/");
    mkdirIfNotExists("/tmp/models/");

    FILE *tmpf;
    makeFileIfNotExists("scene.json", scene_json_data, scene_json_size);
    makeFileIfNotExists("data.txt", data_txt_data, data_txt_size);
    makeFileIfNotExists("data.xlsx", data_xlsx_data, data_xlsx_size);
  }

  return true;
}
