#include "binio/read.hpp"
#include "binio/write.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct NonTrivial {
  ~NonTrivial() {}
  std::uint32_t value;
};

template <typename T> class can_read {
  template <typename U>
  static auto test(int) -> decltype(binio::read(std::declval<std::istream &>(),
                                                std::declval<U &>()),
                                    std::true_type());

  template <typename> static std::false_type test(...);

public:
  static const bool value = decltype(test<T>(0))::value;
};

template <typename T> class can_write {
  template <typename U>
  static auto test(int) -> decltype(binio::write(std::declval<std::ostream &>(),
                                                 std::declval<const U &>()),
                                    std::true_type());

  template <typename> static std::false_type test(...);

public:
  static const bool value = decltype(test<T>(0))::value;
};

static_assert(can_read<std::uint32_t>::value,
              "read should accept scalar types");
static_assert(can_write<std::uint32_t>::value,
              "write should accept scalar types");
static_assert(can_read<double>::value,
              "read should accept floating-point scalar types");
static_assert(can_write<double>::value,
              "write should accept floating-point scalar types");
static_assert(can_read<std::uint32_t[2]>::value,
              "read should accept C array types of scalar elements");
static_assert(can_write<std::uint32_t[2]>::value,
              "write should accept C array types of scalar elements");
static_assert(!can_read<std::uint32_t *>::value,
              "read should reject pointer types");
static_assert(!can_write<std::uint32_t *>::value,
              "write should reject pointer types");
static_assert(!can_read<std::pair<std::uint32_t, std::uint32_t>>::value,
              "read should reject non-scalar aggregate types");
static_assert(!can_write<std::pair<std::uint32_t, std::uint32_t>>::value,
              "write should reject non-scalar aggregate types");
static_assert(!can_read<NonTrivial>::value,
              "read should reject non-trivially-copyable types");
static_assert(!can_write<NonTrivial>::value,
              "write should reject non-trivially-copyable types");

bool test_integer_roundtrip() {
  std::stringstream stream;
  const std::uint32_t expected = 0x12345678u;
  std::uint32_t actual = 0u;

  binio::write(stream, expected);
  stream.seekg(0);
  binio::read(stream, actual);

  return actual == expected;
}

bool test_default_integer_encoding_is_little_endian() {
  std::stringstream stream;
  const std::uint32_t expected = 0x12345678u;

  binio::write(stream, expected);
  const std::string bytes = stream.str();

  return bytes.size() == sizeof(expected) &&
         static_cast<unsigned char>(bytes[0]) == 0x78u &&
         static_cast<unsigned char>(bytes[1]) == 0x56u &&
         static_cast<unsigned char>(bytes[2]) == 0x34u &&
         static_cast<unsigned char>(bytes[3]) == 0x12u;
}

bool test_big_endian_integer_encoding() {
  std::stringstream stream;
  const std::uint32_t expected = 0x12345678u;
  std::uint32_t actual = 0u;

  binio::write(stream, expected, binio::Endianness::Big);
  const std::string bytes = stream.str();
  if (!(bytes.size() == sizeof(expected) &&
        static_cast<unsigned char>(bytes[0]) == 0x12u &&
        static_cast<unsigned char>(bytes[1]) == 0x34u &&
        static_cast<unsigned char>(bytes[2]) == 0x56u &&
        static_cast<unsigned char>(bytes[3]) == 0x78u)) {
    return false;
  }

  stream.seekg(0);
  binio::read(stream, actual, binio::Endianness::Big);
  return actual == expected;
}

bool test_partial_read_sets_failbit() {
  std::stringstream stream;
  const std::uint8_t one_byte = 0xABu;
  std::uint32_t value = 0u;

  binio::write(stream, one_byte);
  stream.seekg(0);
  binio::read(stream, value);

  return stream.fail();
}

bool test_integer_array_roundtrip() {
  std::stringstream stream;
  const std::uint16_t expected[3] = {0x1234u, 0xABCDu, 0x00FFu};
  std::uint16_t actual[3] = {0u, 0u, 0u};

  binio::write(stream, expected, binio::Endianness::Big);
  stream.seekg(0);
  binio::read(stream, actual, binio::Endianness::Big);

  for (std::size_t i = 0; i < 3; ++i) {
    if (actual[i] != expected[i]) {
      return false;
    }
  }

  return true;
}

} // namespace

int main() {
  if (!test_integer_roundtrip()) {
    std::cerr << "test_integer_roundtrip failed\n";
    return 1;
  }

  if (!test_default_integer_encoding_is_little_endian()) {
    std::cerr << "test_default_integer_encoding_is_little_endian failed\n";
    return 1;
  }

  if (!test_big_endian_integer_encoding()) {
    std::cerr << "test_big_endian_integer_encoding failed\n";
    return 1;
  }

  if (!test_partial_read_sets_failbit()) {
    std::cerr << "test_partial_read_sets_failbit failed\n";
    return 1;
  }

  if (!test_integer_array_roundtrip()) {
    std::cerr << "test_integer_array_roundtrip failed\n";
    return 1;
  }

  return 0;
}
