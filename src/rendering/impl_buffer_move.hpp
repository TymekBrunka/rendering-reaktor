#pragma once
#include <utility>

#define __RR_impl_buffer_move(clas, type, movefunc)                           \
                                                                     \
  clas::clas(clas &&other) noexcept : BufferBase(std::move(other)) { \
    movefunc;                                                          \
  }                                                                  \
                                                                     \
  clas &clas::operator=(clas &&other) noexcept {                     \
    if (this != &other) {                                            \
      this->id = other.id;                                           \
      other.id = BufferBase::INVALID_ID;                             \
      movefunc;                                                        \
    }                                                                \
    return *this;                                                    \
  }
