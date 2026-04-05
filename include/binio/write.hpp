#ifndef BINIO_WRITE_HPP
#define BINIO_WRITE_HPP

#include <ostream>

namespace binio {

template <typename T> void write(std::ostream &stream, const T &value) {
  stream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

} // namespace binio

#endif // BINIO_WRITE_HPP
