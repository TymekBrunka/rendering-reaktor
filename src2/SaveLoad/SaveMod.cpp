#include "zipconf.h"
#include <App.hpp>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <yyjson.h>
#include <zip.h>

#include <dirent.h>
#include <errno.h>
#include <filesystem>

#include "Format.hpp"
#include <first_launch_data.h>

char *home_dir = NULL;
char cwd_path[1024] = {0};
char formated_path[1024] = {0};
char formated_path_2[1024] = {0}; // updates more frequently, used internally in abstract_file functions
char c_path[1024] = {0};

zip_t *open_archive;
zip_error_t *zerror;

#include "Format_0_0_1.cpp"

#define SCOPE(...)

bool json_get_vector3(yyjson_val *val, float *x, float *y, float *z) {
  if (!yyjson_is_arr(val))
    return false;

  if (yyjson_arr_size(val) != 3)
    return false;

  yyjson_val *x_val = yyjson_arr_get(val, 0);
  yyjson_val *y_val = yyjson_arr_get(val, 1);
  yyjson_val *z_val = yyjson_arr_get(val, 2);

  if (!yyjson_is_num(x_val) || !yyjson_is_num(y_val) || !yyjson_is_num(z_val))
    return false;

  *x = (float)yyjson_get_num(x_val);
  *y = (float)yyjson_get_num(y_val);
  *z = (float)yyjson_get_num(z_val);
  return true;
}

abstract_memory abstract_memory_create(bool is_raw, void *data, size_t size) {
  abstract_memory memory{0, NULL, NULL};
  if (is_raw) {
    memory.size = size;
    memory.data = (char *)data;
    memory.source = NULL;
    return memory;
  } else {
    zip_source_t *src;
    if ((src = zip_source_buffer_create(data, size, 1, zerror)) == NULL) {
      // fprintf(stderr, "Can't create source: %s\n", zip_error_strerror(&error));
      // zip_error_fini(&zerror);
      return memory;
    }
    memory.size = size;
    memory.source = src;
    return memory;
  }
}

bool abstract_memory_read_data(abstract_memory *memory) {
  if (memory->data != NULL) {
    return true;
  }

  if (memory->source == NULL)
    return false;

  if (zip_source_open(memory->source) == -1) {
    // fprintf(stderr, "Cannot open source for 'data.txt' from zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    return false;
  }

  char *data = new char[memory->size + 1];
  if (zip_source_read(memory->source, data, memory->size) <= 0) {
    // fprintf(stderr, "Cannot read contents of 'data.txt' from zip archive : %s\n", zip_error_strerror(zip_source_error(src)));
    delete[] data;
    zip_source_close(memory->source);
    return false;
  }

  if (zip_source_close(memory->source) < 0) {
    fprintf(stderr, "Cannot close zip source: %s\n", zip_error_strerror(zip_source_error(memory->source)));
    return true; // ngl, not sure if that's ok if source reads succesfully but cannot be closed
  }

  data[memory->size] = '\0';
  memory->data = data;
  return true;
}

void abstract_memory_free(abstract_memory *memory) {
  if (memory->data)
    delete[] memory->data;

  if (memory->source)
    zip_source_free(memory->source);
}

abstract_memory abstract_file_read(abstract_file *afile) {
  abstract_memory memory{0, NULL, NULL};

  if (afile->is_file) {
    if (fseek(afile->u.file, 0, SEEK_END)) {
      fprintf(stderr, "fseek failed on file %s\n", formated_path_2);
      return memory;
    }

    size_t filesize = ftell(afile->u.file);
    fseek(afile->u.file, 0, SEEK_SET);

    char *blob = new char[filesize + 1];
    fread(blob, filesize, 1, afile->u.file);
    blob[filesize] = '\0';
    // fclose(input);

    memory = abstract_memory_create(true, blob, filesize);
    return memory;
  } else {
    zip_source_t *src = zip_source_zip_file(open_archive, open_archive, afile->u.stat.index, 0, 0, -1, NULL);

    if (src == NULL) {
      // fprintf(stderr, "Cannot create source for '%s' from zip archive\n", afile->path);
      return memory;
    }

    if (afile->u.stat.name == NULL)
      if (zip_source_stat(src, &afile->u.stat) < 0) {
        // fprintf(stderr, "Can't stat file '%s' in zip archive : %s\n", afile->path, zip_error_strerror(zip_source_error(src)));
        zip_source_free(src);
        return memory;
      }

    memory.source = src;
    memory.size = afile->u.stat.size;
    return memory;
  }
}

abstract_memory abstract_file_open_and_read(abstract_file *afile, const char *path) {
  afile->path = (char *)path;
  abstract_memory memory{0, NULL, NULL};

  if (afile->is_file) {
    snprintf(formated_path_2, 1024, "%s" SEP "%s", cwd_path, afile->path);
    fprintf(stderr, "%s\n", formated_path_2);
#ifdef _WIN32
    for (char *c = formated_path_2; *c != '\0'; c++)
      if (*c == '/')
        *c = '\\';
#endif

    FILE *input = fopen(formated_path_2, "rb");

    if (!input) {
      fprintf(stderr, "Couldn't open file %s for reading\n", formated_path_2);
      return memory;
    }
    afile->u.file = input;

  } else {

    zip_stat_t stat;
    zip_stat_init(&stat);

    zip_int64_t idx = zip_name_locate(open_archive, path, 0);
    // fprintf(stderr, "%s found at idx == %lld\n", afile->path, idx);
    if (idx == -1) {
      // fprintf(stderr, "Zip archive doesnt contain '%s'\n", afile->path);;
      return memory;
    }

    stat.index = idx;
    afile->u.stat = stat;
  }

  memory = abstract_file_read(afile);
  return memory;
};

bool abstract_file_close(abstract_file *afile) {
  if (afile->is_file)
    return !fclose(afile->u.file);

  return true;
}

bool abstract_file_make_real(abstract_file *afile, abstract_memory *memory) {
  snprintf(formated_path_2, 1024, "%s" SEP "%s", cwd_path, afile->path);
#ifdef _WIN32
  for (char *c = formated_path_2; *c != '\0'; c++)
    if (*c == '/')
      *c = '\\';
#endif

  if (afile->is_file) {
    FILE *output = fopen(formated_path_2, "rb");
    if (output) {
      fclose(output);
      return true;
    }
  }

  if (!abstract_memory_read_data(memory)) {
    // fprintf(stderr, "Couldnt read data from abstract memory\n");
    return false;
  }

  FILE *output = fopen(formated_path_2, "wb");

  if (!output) {
    // fprintf(stderr, "Couldn't open file %s for writing\n", formated_path_2);
    return false;
  }

  fwrite(memory->data, 1, memory->size, output);
  fclose(output);
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

#define if_field_is(x)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
  if (reader + sizeof((x)) - 1 <= reader_end + 1)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (!strncmp(reader, (x), sizeof((x)) - 1))

bool App::read_data_txt(char *data_txt, size_t data_txt_len, AppMetadata *meta) {
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

#define endzip()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_source_free(src);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  delete[] blob;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  zip_error_fini(&error);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
  return false;

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

  open_archive = za;
  zerror = &error;
  snprintf(cwd_path, 1024, "%s" ROOTDIR TMPDIR, home_dir);

  load_app(true, cwd_path);

  zip_source_free(src);
  // delete[] blob;
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

void wstr2cstr(const void *wstr) {
#ifdef _WIN32
  size_t len = wcstombs(nullptr, (const wchar_t *)wstr, 0) + 1;
  // char *buffer = new char[len];
  memset(c_path, 0, 1024);
  wcstombs(c_path, (const wchar_t *)wstr, len);
#else
  size_t len = strlen((const char *)wstr);
  // char *buffer = new char[len + 1];
  memcpy(c_path, wstr, len);
  c_path[len] = '\0';
#endif
}

bool App::load_app(bool from_zip, const char *root) {
  if (root != cwd_path)
    snprintf(cwd_path, 1024, "%s", root);

  SCOPE("handle first launch / missing files") {

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

  zip_stat_t stat;
  zip_stat_init(&stat);

  SavableState tmp_state;
  tmp_state.setup();

  AppMetadata meta;
  SCOPE("reading contents of data.txt") {

    abstract_file data_txt_afile{!from_zip};
    abstract_memory data_txt_mem = abstract_file_open_and_read(&data_txt_afile, "data.txt");
    if (data_txt_mem.data == NULL && data_txt_mem.source == NULL) {
      fprintf(stderr, "Couldn't open (abstract)file data.txt for reading\n");
      return false;
    }

    if (!abstract_memory_read_data(&data_txt_mem)) {
      fprintf(stderr, "Couldn't read data of (abstract)file data.txt\n");
      abstract_memory_free(&data_txt_mem);
      return false;
    }

    if (!read_data_txt(data_txt_mem.data, data_txt_mem.size, &meta)) {
      abstract_memory_free(&data_txt_mem);
      // endzip();
      return false;
    }

    abstract_memory_free(&data_txt_mem);
  }

  bool (*loader)(SavableState *, AppMetadata *, abstract_file *) = NULL;

  SCOPE("selecting proper loader") {

    if (meta.format_major == 0) {
      if (meta.format_minor == 0 && meta.format_patch == 1) {
        loader = loader_0_0_1;
      }
    }

    if (loader == NULL) {
      fprintf(stderr, "Couldnt find loader for format version %d.%d.%d\n", meta.format_major, meta.format_minor, meta.format_patch);
      // endzip();
      return false;
    }
  }

  bool (*scene_loader)(SavableState *, AppMetadata *, yyjson_val *) = NULL;

  SCOPE("selecting proper scene loader") {

    if (meta.format_major == 0) {
      if (meta.format_minor == 0 && meta.format_patch == 1) {
        scene_loader = scene_loader_0_0_1;
      }
    }

    if (scene_loader == NULL) {
      fprintf(stderr, "Couldnt find scene loader for format version %d.%d.%d\n", meta.format_major, meta.format_minor, meta.format_patch);
      // endzip();
      return false;
    }
  }
  abstract_memory scene_json_mem;

  SCOPE("reading contents of scene.json") {

    abstract_file scene_json_afile{!from_zip};
    scene_json_mem = abstract_file_open_and_read(&scene_json_afile, "scene.json");
    if (scene_json_mem.data == NULL && scene_json_mem.source == NULL) {
      fprintf(stderr, "Couldn't open (abstract)file scene.json for reading\n");
      return false;
    }

    if (!abstract_memory_read_data(&scene_json_mem)) {
      fprintf(stderr, "Couldn't read data of (abstract)file scene.json\n");
      abstract_memory_free(&scene_json_mem);
      return false;
    }

    fprintf(stderr, "Contents of 'scene.json':\n>>>>\n%s\n<<<<\n", scene_json_mem.data);
  }

  SCOPE("reading files") {

    if (from_zip) {
      // from zip
      size_t num_of_entries = zip_get_num_entries(open_archive, 0);
      fprintf(stderr, "-- NUMBER OF ENTRIES: %llu\n", num_of_entries);
      for (size_t i = 0; i < num_of_entries; i++) {

        abstract_file afile{false};
        if (zip_stat_index(open_archive, i, 0, &afile.u.stat) < 0) {
          fprintf(stderr, "Can't stat file %llu in zip archive : %s\n", i, zip_strerror(open_archive));
          continue;
        }
        afile.path = (char *)afile.u.stat.name;

        if (!strcmp(afile.path, "data.txt") || !strcmp(afile.path, "scene.json") || !strcmp(afile.path, "data.xlsx"))
          continue;

        fprintf(stderr, "file: %s\n", stat.name);

        if (!loader(&tmp_state, &meta, &afile)) {
          // endzip();
          abstract_memory_free(&scene_json_mem);
          return false;
        }
      }
    } else {
      // from files
      size_t cwd_path_len = strlen(cwd_path);
      for (const auto &ent : std::filesystem::recursive_directory_iterator(cwd_path)) {
        if (std::filesystem::is_regular_file(ent.path())) {
          wstr2cstr(ent.path().c_str()); // ent.path().c_str() doesnt return c string but os-specific type so on windows i have to convert it to regular c string

          abstract_file afile{true, &c_path[cwd_path_len]};
          FILE *file = fopen(c_path, "rb");
          if (!file) {
            fprintf(stderr, "Couldnt open file %s for reading\n", c_path);
            return false;
          }
          // fix path for integrity with libzip
#ifdef _WIN32
          for (char *c = &c_path[cwd_path_len]; *c != '\0'; c++)
            if (*c == '\\')
              *c = '/';
#endif

          if (!strcmp(afile.path, "data.txt") || !strcmp(afile.path, "scene.json") || !strcmp(afile.path, "data.xlsx")) {
            // delete[] c_path;
            continue;
          }

          fprintf(stderr, "file: %s\n", &c_path[cwd_path_len]);

          if (!loader(&tmp_state, &meta, &afile)) {
            abstract_memory_free(&scene_json_mem);
            // delete[] c_path;
            return false;
          }
          // delete[] c_path;
        }
      }
    }
  }

  SCOPE("importing scene") {
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_opts(scene_json_mem.data, scene_json_mem.size, YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS, 0, &err);
    if (!doc) {
      fprintf(stderr, "read error: %s, code: %u at byte position: %llu\n", err.msg, err.code, err.pos);
      yyjson_doc_free(doc);
      abstract_memory_free(&scene_json_mem);
      return false;
    }

    yyjson_val *root_val = yyjson_doc_get_root(doc);
    if (!root_val) {
      fprintf(stderr, "failed to load scene data\n");
      yyjson_doc_free(doc);
      abstract_memory_free(&scene_json_mem);
      return false;
    }

    if (!scene_loader(&tmp_state, &meta, root_val)) {
      fprintf(stderr, "failed to load scene data\n");
      yyjson_doc_free(doc);
      abstract_memory_free(&scene_json_mem);
      return false;
    }

    yyjson_doc_free(doc);
  }

  abstract_memory_free(&scene_json_mem);

  metadata = meta;
  state = std::move(tmp_state);

  return true;
}

bool App::save_app() {
  SCOPE("copy files to root") {
    size_t cwd_path_len = strlen(cwd_path);
    snprintf(formated_path, 1024, "%s" ROOTDIR, home_dir);
    if (memcmp(cwd_path, formated_path, 1024)) {
      for (const auto &ent : std::filesystem::recursive_directory_iterator(cwd_path)) { // cwd_path/tmp
        wstr2cstr(ent.path().c_str()); // ent.path().c_str() doesnt return c string but os-specific type so on windows i have to convert it to regular c string
        snprintf(formated_path, 1024, "%s" ROOTDIR "%s", home_dir, &c_path[cwd_path_len]);
        fprintf(stderr, "fmt pth: %s\n", formated_path);
        fprintf(stderr, "cwd_path: %s\n", cwd_path);
        fprintf(stderr, "cpath pth: %s\n", &c_path[cwd_path_len]);
        // std::filesystem::rename(ent.path(), formated_path);
        if (std::filesystem::is_directory(ent.path())) {
          DIR *dir = opendir(formated_path);
          if (dir)
            closedir(dir);
          else if (errno == ENOENT) {
            std::filesystem::create_directory(formated_path);
          }
        }
        if (std::filesystem::is_regular_file(ent.path())) {
          FILE* input = fopen(c_path, "rb");
          
          if (!input) {
            fprintf(stderr, "Couldn't open file %s for reading\n", c_path);
            return false;
          }

          if (fseek(input, 0, SEEK_END)) {
            fprintf(stderr, "fseek failed on file %s\n", c_path);
            return false;
          }

          size_t filesize = ftell(input);
          fseek(input, 0, SEEK_SET);

          char* blob = new char[filesize];
          fread(blob, filesize, 1, input);
          fclose(input);

          FILE* output = fopen(formated_path, "wb");

          if (!output) {
            fprintf(stderr, "Couldn't open file %s for writing\n", formated_path);
            return false;
          }

          fwrite(blob, 1, filesize, output);
          fclose(output);
        }
      }
    } 
  }

  snprintf(formated_path, 1024, "%s" ROOTDIR "data.txt", home_dir);
  FILE* data_txt = fopen(formated_path, "wb");

  if (!data_txt) {
    fprintf(stderr, "Couldn't open file %s for writing\n", formated_path);
  }
  fwrite(data_txt_data, 1, data_txt_size, data_txt);
  fclose(data_txt);

  SCOPE("saving scene") {
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_val *objects_val = yyjson_mut_obj_add_arr(doc, root, "objects");
    for (const auto& object : state.objects) {
      yyjson_mut_val *object_val = yyjson_mut_arr_add_obj(doc, objects_val);
      yyjson_mut_obj_add_str(doc, object_val, "model", object.model_ref.name.c_str());
    }
  }

  return true;
}
