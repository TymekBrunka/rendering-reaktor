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
  void *data;
  zip_source_t *source;
};

extern char *home_dir;
extern char cwd_path[1024];
extern char formated_path[1024];

bool abstract_file_open_and_read(const char *path, abstract_file *afile, size_t *size);
abstract_memory abstract_file_read(abstract_file *afile, size_t *size);
bool abstract_file_write(abstract_file *afile, size_t size, void *data);
bool abstract_file_make_real(abstract_file *afile);

#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif

#define ROOTDIR SEP ".reaktory" SEP
#define TMPDIR "tmp" SEP
#define MODELS_DIR "models" SEP
