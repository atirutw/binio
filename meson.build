project('binio', 'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++11',
    ],
)

binio_dep = declare_dependency(
    include_directories: include_directories('include')
)

subdir('tests')

install_headers(
    'include/binio/endianness.hpp',
    'include/binio/read.hpp',
    'include/binio/write.hpp',
    subdir: 'binio',
)

meson.override_dependency('binio', binio_dep)
