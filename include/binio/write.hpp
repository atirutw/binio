#ifndef BINIO_WRITE_HPP
#define BINIO_WRITE_HPP

#include "binio/endianness.hpp"

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

} // namespace binio

#endif // BINIO_WRITE_HPP
