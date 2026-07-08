#include <stdint.h>
#include <stdio.h>
#include <string.h>

char formated_string[1024] = {0};
char c_file_path[200] = {0};
char h_file_path[200] = {0};
char header_guard_text[200] = {0};

void get_file_name(const char *filepath, char *out) {
  size_t start = 0;
  size_t length = strlen(filepath);
  for (intptr_t i = length - 1; i >= 0; i--) {
    if (filepath[i] == '/'
#ifdef _WIN32
        || filepath[i] == '\\'
#endif
    ) {
      start = i + 1;
      break;
    }
  }

  // char *name = calloc(1, length - start + 1);
  memcpy(out, &filepath[start], length - start);
}

void name2cident(char *name, char* out) {
  char *c = name;
  while (*c != '\0') {
    *out = *c;
    if ((*c < '0' || *c > '9') && (*c < 'a' || *c > 'z') && (*c < 'A' || *c > 'Z'))
      *out = '_';
    c++;
    out++;
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "bytepack: Not enough arguments\n");
    return 1;
  }

  snprintf(c_file_path, 1024, "%s.c", argv[1]);
  FILE *cfile = fopen(c_file_path, "wb");
  snprintf(h_file_path, 1024, "%s.h", argv[1]);
  FILE *hfile = fopen(h_file_path, "wb");

  if (!cfile) {
    fprintf(stderr, "bytepack: Cannot open file %s for writing\n", c_file_path);
    return 1;
  }

  if (!hfile) {
    fprintf(stderr, "bytepack: Cannot open file %s for writing\n", h_file_path);
    return 1;
  }

  get_file_name(h_file_path, formated_string);
  fprintf(cfile, "#include \"%s\"\n\n", formated_string);
  name2cident(formated_string, formated_string);
  fprintf(hfile, "#ifndef %s\n#define %s\n\n", formated_string, formated_string);

  fprintf(hfile, "#endif");
  fclose(hfile);
  fclose(cfile);
}
