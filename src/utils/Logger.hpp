#pragma once
#include <format>
#include <glad/glad.h>
#include <iostream>
#include <ostream>
#include <source_location>

template <std::basic_ostream<char> &stream = std::cout>
class Logger {
public:
  static std::basic_ostream<char> &info(const char *ctx_name, std::source_location srcl = std::source_location::current()) {
    stream << std::format("\x1b[32m[\x1b[34m  INFO \x1b[32m][\x1b[34m {} \x1b[32m][\x1b[33m{}:{}:{}\x1b[32m]\x1b[0m ", ctx_name, srcl.function_name(), srcl.line(), srcl.column());
    return stream;
  }

  static std::basic_ostream<char> &warn(const char *ctx_name, std::source_location srcl = std::source_location::current()) {
    stream << std::format("\x1b[32m[\x1b[33mWARNING\x1b[33m][\x1b[34m {} \x1b[32m][\x1b[33m{}:{}:{}\x1b[32m]\x1b[0m ", ctx_name, srcl.function_name(), srcl.line(), srcl.column());
    return stream;
  }

  static std::basic_ostream<char> &error(const char *ctx_name, std::source_location srcl = std::source_location::current()) {
    stream << std::format("\x1b[32m[\x1b[31m ERROR \x1b[32m][\x1b[34m {} \x1b[32m][\x1b[33m{}:{}:{}\x1b[32m]\x1b[0m ", ctx_name, srcl.function_name(), srcl.line(), srcl.column());
    return stream;
  }
};
