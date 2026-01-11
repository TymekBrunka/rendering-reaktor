#include <glad/glad.h>
#include <source_location>
#include <iostream>
#include <ostream>
#include <format>

namespace Logger {
  std::basic_ostream<char>& info(const char* ctx_name, std::source_location srcl = std::source_location::current());
  std::basic_ostream<char>& warn(const char* ctx_name, std::source_location srcl = std::source_location::current());
  std::basic_ostream<char>& error(const char* ctx_name, std::source_location srcl = std::source_location::current());
};
