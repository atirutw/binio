#ifndef BINIO_WRITE_HPP
#define BINIO_WRITE_HPP

#include "binio/endianness.hpp"

#include <cstddef>
#include <ostream>
#include <type_traits>

namespace binio {

template <typename T,
          typename std::enable_if<(std::is_arithmetic<T>::value &&
                                   !std::is_pointer<T>::value) ||
                                  std::is_enum<T>::value>::type * = nullptr>
void write(std::ostream &stream, const T &value,
           Endianness endianness = Endianness::Little) {
  T storage = value;
  if (detail::should_swap_bytes(endianness, sizeof(T))) {
    storage = detail::byte_swap(storage);
  }

  stream.write(reinterpret_cast<const char *>(&storage), sizeof(T));
}

template <typename T, std::size_t N,
          typename std::enable_if<(std::is_arithmetic<T>::value &&
                                   !std::is_pointer<T>::value) ||
                                  std::is_enum<T>::value>::type * = nullptr>
void write(std::ostream &stream, const T (&values)[N],
           Endianness endianness = Endianness::Little) {
  for (std::size_t i = 0; i < N; ++i) {
    write(stream, values[i], endianness);
    if (stream.fail()) {
      return;
    }
  }
}

} // namespace binio

#endif // BINIO_WRITE_HPP
