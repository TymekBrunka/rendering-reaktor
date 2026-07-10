#pragma once
#include <cstddef>
#include <zip.h>

struct abstract_file {
  bool is_file;
  char *path;
  union {
    zip_stat_t stat;
    FILE *file;
  } u;
};

struct abstract_memory {
  size_t size;
  char *data;
  zip_source_t *source;
};

extern char *home_dir;
extern char cwd_path[1024];
extern char formated_path[1024];
extern char formated_path_2[1024]; // updates more frequently, used internally in abstract_file functions

extern zip_t *open_archive;
extern zip_error_t *zerror;

abstract_memory abstract_memory_create(bool is_raw, void *data, size_t size);
bool abstract_memory_read_data(abstract_memory *memory);
void abstract_memory_free(abstract_memory *memory);

abstract_memory abstract_file_read(abstract_file *afile);
abstract_memory abstract_file_open_and_read(abstract_file *afile, const char *path);
bool abstract_file_close(abstract_file *afile);
bool abstract_file_write(abstract_file *afile, abstract_memory);
bool abstract_file_make_real(abstract_file *afile);

#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif

#define ROOTDIR SEP ".reaktory" SEP
#define TMPDIR "tmp" SEP
#define MODELS_DIR "models" SEP
