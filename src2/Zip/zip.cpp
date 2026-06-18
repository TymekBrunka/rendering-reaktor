#include <iostream>
#include <zip.h>

int main(int argc, const char *argv[]) {
  std::cout << "loading file: " << argv[1] << "\n";

  zip_error_t error;
  zip_error_init(&error);

  int errop;
  zip_t *archive;
  if ((archive = zip_open("assets/test.zip", ZIP_RDONLY, &erropt)) == NULL) {
    zip_error_t error;
    zip_error_init_with_code(&error, err);
    std::cout << "cannot open zip archive " << argv[1] << " : " zip_error_strerror(&error) << "\n";
    zip_error_fini(&error);
    return -1;
  }
}
