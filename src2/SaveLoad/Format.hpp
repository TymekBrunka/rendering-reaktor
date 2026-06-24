#pragma once
#include <zip.h>

extern char *home_dir;
extern char formated_path[1024];
bool alloc_read_file_from_zip(zip_t *za, zip_int64_t idx, char **data, zip_source_t *src, zip_stat_t *stat);

#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif

#define ROOTDIR SEP "reaktory" SEP
#define TMPDIR ROOTDIR "tmp" SEP
#define MODELS_TMPDIR TMPDIR "models" SEP
