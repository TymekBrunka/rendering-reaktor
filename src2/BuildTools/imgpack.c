#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
  out[length] = '\0';
}

void name2cident(char *name, char *out) {
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
  if (argc < 4) {
    fprintf(stderr, "imgpack: Not enough arguments\n");
    return 1;
  }

  snprintf(c_file_path, 200, "%s.c", argv[1]);
  FILE *cfile = fopen(c_file_path, "wb");
  snprintf(h_file_path, 200, "%s.h", argv[1]);
  FILE *hfile = fopen(h_file_path, "wb");

  if (!cfile) {
    fprintf(stderr, "imgpack: Cannot open file %s for writing\n", c_file_path);
    return 1;
  }

  if (!hfile) {
    fprintf(stderr, "imgpack: Cannot open file %s for writing\n", h_file_path);
    return 1;
  }

  get_file_name(h_file_path, formated_string);
  fprintf(cfile, "#include \"%s\"\n\n", formated_string);
  name2cident(formated_string, formated_string);
  fprintf(hfile,
          "#ifndef %s\n"
          "#define %s\n"
          "#include <stdlib.h>\n\n",
          formated_string, formated_string);

  for (int i = 3; i < argc; i++) {
    int width, height, channels;
    unsigned char *pixels = stbi_load(argv[i], &width, &height, &channels, atoi(argv[2]));
    if (pixels == NULL) {
      fprintf(stderr, "imgpack: skipped file %s\n", argv[i]);
      continue;
    }
    size_t pixels_amount = width * height * channels;
    get_file_name(argv[i], formated_string);
    name2cident(formated_string, formated_string);
    fprintf(hfile,
            "extern int %s_width;\n"
            "extern int %s_height;\n"
            "extern int %s_channels;\n"
            "extern unsigned char %s_pixels[%ld];\n\n",
            formated_string, formated_string, formated_string, formated_string, pixels_amount);

    fprintf(cfile,
            "int %s_width = %d;\n"
            "int %s_height = %d;\n"
            "int %s_channels = %d;\n"
            "unsigned char %s_pixels[%ld] = {",
            formated_string, width, formated_string, height, formated_string, channels, formated_string, pixels_amount);

    unsigned char *end = &pixels[pixels_amount - 1];
    for (unsigned char *uc = pixels; uc <= end; uc++) {
      fprintf(cfile, "0x%0.2x, ", *uc);
    }
    fprintf(cfile, "};\n\n");
  }

  fprintf(hfile, "#endif");
  fclose(hfile);
  fclose(cfile);

  fprintf(stderr, "imgpack: \x1b[32msuccesfully wrote files \x1b[34m%s\x1b[32m and \x1b[34m%s\x1b[0m\n", c_file_path, h_file_path);

  return 0;
}
