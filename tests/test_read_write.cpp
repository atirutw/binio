#include "binio/read.hpp"
#include "binio/write.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <utility>

namespace {

struct Sample {
  std::uint16_t a;
  std::uint32_t b;
};

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
              "read should accept trivially copyable scalar types");
static_assert(can_write<std::uint32_t>::value,
              "write should accept trivially copyable scalar types");
static_assert(can_read<Sample>::value,
              "read should accept trivially copyable struct types");
static_assert(can_write<Sample>::value,
              "write should accept trivially copyable struct types");
static_assert(!can_read<std::uint32_t *>::value,
              "read should reject pointer types");
static_assert(!can_write<std::uint32_t *>::value,
              "write should reject pointer types");
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

bool test_struct_roundtrip() {
  std::stringstream stream;
  const Sample expected = {0x00FFu, 0xCAFEBABEu};
  Sample actual = {0u, 0u};

  binio::write(stream, expected);
  stream.seekg(0);
  binio::read(stream, actual);

  return actual.a == expected.a && actual.b == expected.b;
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

} // namespace

int main() {
  if (!test_integer_roundtrip()) {
    std::cerr << "test_integer_roundtrip failed\n";
    return 1;
  }

  if (!test_struct_roundtrip()) {
    std::cerr << "test_struct_roundtrip failed\n";
    return 1;
  }

  if (!test_partial_read_sets_failbit()) {
    std::cerr << "test_partial_read_sets_failbit failed\n";
    return 1;
  }

  return 0;
}
