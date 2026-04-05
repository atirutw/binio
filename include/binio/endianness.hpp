#ifndef BINIO_ENDIANNESS_HPP
#define BINIO_ENDIANNESS_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace binio {

enum class Endianness { Little, Big };

namespace detail {

inline bool is_native_little_endian() {
  const std::uint16_t value = 0x0001u;
  return *reinterpret_cast<const unsigned char *>(&value) == 0x01u;
}

inline bool should_swap_bytes(Endianness endianness, std::size_t size) {
  if (size == 1u) {
    return false;
  }

  const bool native_little = is_native_little_endian();
  if (endianness == Endianness::Little) {
    return !native_little;
  }

  return native_little;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 1u, T>::type
byte_swap(T value) {
  return value;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 2u, T>::type
byte_swap(T value) {
  typedef typename std::make_unsigned<T>::type U;
  const U u = static_cast<U>(value);
  const U swapped = static_cast<U>((u >> 8u) | (u << 8u));
  return static_cast<T>(swapped);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 4u, T>::type
byte_swap(T value) {
  typedef typename std::make_unsigned<T>::type U;
  const U u = static_cast<U>(value);
  const U swapped =
      static_cast<U>(((u & 0x000000FFu) << 24u) | ((u & 0x0000FF00u) << 8u) |
                     ((u & 0x00FF0000u) >> 8u) | ((u & 0xFF000000u) >> 24u));
  return static_cast<T>(swapped);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8u, T>::type
byte_swap(T value) {
  typedef typename std::make_unsigned<T>::type U;
  const U u = static_cast<U>(value);
  const U swapped = static_cast<U>(((u & 0x00000000000000FFull) << 56u) |
                                   ((u & 0x000000000000FF00ull) << 40u) |
                                   ((u & 0x0000000000FF0000ull) << 24u) |
                                   ((u & 0x00000000FF000000ull) << 8u) |
                                   ((u & 0x000000FF00000000ull) >> 8u) |
                                   ((u & 0x0000FF0000000000ull) >> 24u) |
                                   ((u & 0x00FF000000000000ull) >> 40u) |
                                   ((u & 0xFF00000000000000ull) >> 56u));
  return static_cast<T>(swapped);
}

template <typename T>
typename std::enable_if<std::is_enum<T>::value, T>::type byte_swap(T value) {
  typedef typename std::underlying_type<T>::type U;
  return static_cast<T>(byte_swap(static_cast<U>(value)));
}

} // namespace detail

} // namespace binio

#endif // BINIO_ENDIANNESS_HPP
