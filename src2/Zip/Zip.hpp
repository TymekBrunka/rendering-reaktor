#pragma once
#include <memory>

class Zip {
private:
  const char *comment;
  std::shared_ptr<void *> error_handle;

public:
  Zip() = default;
  Zip(const char* filepath);
  Zip(const char* data, int size);

  const char* get_comment() const;
  void* get_entry(const char* filepath);
};
