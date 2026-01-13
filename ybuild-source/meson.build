project('yaml-build', 'cpp',
  version: '1.0',
  default_options: [
    'cpp_std=c++23',
    'warning_level=3',
    'optimization=2'
  ]
)

# Dependencies
curl_dep = dependency('libcurl')
yaml_cpp_dep = dependency('yaml-cpp')
ssl_dep = dependency('openssl')
crypto_dep = dependency('libcrypto')
magic_dep = dependency('libmagic', required: false)

# If libmagic is not found via pkg-config, try finding it manually
if not magic_dep.found()
  magic_dep = meson.get_compiler('cpp').find_library('magic')
endif

# Source files
sources = ['main.cpp']

# Shared executable (yaml-build)
shared_deps = [
  curl_dep,
  yaml_cpp_dep,
  ssl_dep,
  crypto_dep,
  magic_dep
]

executable('yaml-build',
  sources,
  dependencies: shared_deps,
  install: true
)

cpp = meson.get_compiler('cpp')
# Static executable (Ybuild)
# For static linking, we need to specify additional dependencies
static_deps = [
  cpp.find_library('curl', static: true),
  cpp.find_library('idn2', static: true),
  cpp.find_library('psl', static: true),
  cpp.find_library('unistring', static: true),
  cpp.find_library('zstd', static: true),
  cpp.find_library('z', static: true),
  cpp.find_library('ssl', static: true),
  cpp.find_library('crypto', static: true),
  cpp.find_library('yaml-cpp', static: true),
  cpp.find_library('magic', static: true),
  cpp.find_library('lzma', static: true),
  cpp.find_library('bz2', static: true)
]

executable('Ybuild',
  sources,
  dependencies: static_deps,
  link_args: ['-static'],
  install: true
)
