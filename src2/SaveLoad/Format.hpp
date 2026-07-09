#pragma once
#include <zip.h>

struct abstract_file {
  bool is_file;
  union {
    zip_stat_t stat;
    FILE* file;
  } u;
};

extern char *home_dir;
extern char cwd_path[1024];
extern char formated_path[1024];

bool alloc_read_file_from_zip(zip_t *za, zip_int64_t idx, char **data, zip_source_t *src, zip_stat_t *stat);
bool make_file_real(abstract_file *afile);

#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif

#define ROOTDIR SEP ".reaktory" SEP
#define TMPDIR "tmp" SEP
#define MODELS_DIR "models" SEP
