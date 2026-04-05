#ifndef BINIO_WRITE_HPP
#define BINIO_WRITE_HPP

#include <ostream>
#include <type_traits>

namespace binio {

template <typename T,
          typename std::enable_if<std::is_trivially_copyable<T>::value &&
                                  !std::is_pointer<T>::value>::type * = nullptr>
void write(std::ostream &stream, const T &value) {
  stream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

} // namespace binio

#endif // BINIO_WRITE_HPP
