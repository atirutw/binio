#ifndef BINIO_READ_HPP
#define BINIO_READ_HPP

#include "binio/endianness.hpp"

#include <cstddef>
#include <istream>
#include <type_traits>

namespace binio {

template <typename T,
          typename std::enable_if<(std::is_arithmetic<T>::value &&
                                   !std::is_pointer<T>::value) ||
                                  std::is_enum<T>::value>::type * = nullptr>
void read(std::istream &stream, T &value,
          Endianness endianness = Endianness::Little) {
  T storage = static_cast<T>(0);
  stream.read(reinterpret_cast<char *>(&storage), sizeof(T));
  if (stream.fail()) {
    return;
  }

  if (detail::should_swap_bytes(endianness, sizeof(T))) {
    storage = detail::byte_swap(storage);
  }

  value = storage;
}

template <typename T, std::size_t N,
          typename std::enable_if<(std::is_arithmetic<T>::value &&
                                   !std::is_pointer<T>::value) ||
                                  std::is_enum<T>::value>::type * = nullptr>
void read(std::istream &stream, T (&values)[N],
          Endianness endianness = Endianness::Little) {
  for (std::size_t i = 0; i < N; ++i) {
    read(stream, values[i], endianness);
    if (stream.fail()) {
      return;
    }
  }
}

} // namespace binio

#endif // BINIO_READ_HPP
