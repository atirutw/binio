#ifndef BINIO_READ_HPP
#define BINIO_READ_HPP

#include <istream>
#include <type_traits>

namespace binio {

template <typename T,
          typename std::enable_if<std::is_trivially_copyable<T>::value &&
                                  !std::is_pointer<T>::value>::type * = nullptr>
void read(std::istream &stream, T &value) {
  stream.read(reinterpret_cast<char *>(&value), sizeof(T));
}

} // namespace binio

#endif // BINIO_READ_HPP
