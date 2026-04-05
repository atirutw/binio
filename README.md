# BinIO
Utility for reading and writing binary data in C++.

## Endianness

- Scalar read and write default to little-endian encoding.
- You can override endianness with `binio::Endianness::Big`.
- This library only supports scalar values. Custom classes and structs should be serialized explicitly field-by-field by consumer code.
