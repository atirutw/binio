#ifndef BINIO_READ_HPP
#define BINIO_READ_HPP

#include <istream>

namespace binio {

template <typename T> void read(std::istream &stream, T &value) {
  stream.read(reinterpret_cast<char *>(&value), sizeof(T));
}

} // namespace binio

#endif // BINIO_READ_HPP
