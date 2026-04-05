project('binio', 'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++11',
    ],
)

binio_dep = declare_dependency(
    include_directories: include_directories('include')
)

if not meson.is_subproject()
    install_headers(
        'include/binio/endianness.hpp',
        'include/binio/read.hpp',
        'include/binio/write.hpp',
        subdir: 'binio',
    )

    subdir('tests')
endif

meson.override_dependency('binio', binio_dep)
