# OpenAssetIO Test Host

A bare-bones C++ host application with [OpenAssetIO](https://github.com/OpenAssetIO/OpenAssetIO)
as a dependency, for testing basic integration (linking, headers, etc).

Test targets are created that simply check that C++, C and
Python components can be imported.

> **Note**
>
> This project is intended for use in CI and not used as a reference.

## Getting started

This application can link against OpenAssetIO in two modes, depending on
the value of the `OPENASSETIOTEST_ENABLE_SUBPROJECT` CMake variable -
see below.

The usual CMake steps can be used to build and run the tests. Assuming
the current working directory is at the root of the repository:

```shell
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

### `OPENASSETIOTEST_ENABLE_SUBPROJECT=OFF` (default)

When `OPENASSETIOTEST_ENABLE_SUBPROJECT=OFF` (which is the default if
unspecified), OpenAssetIO will be discovered as a CMake package
(`find_package(OpenAssetIO)`) and so OpenAssetIO must be pre-built and
installed into one of [find_package](https://cmake.org/cmake/help/v3.21/command/find_package.html)'s
configured search paths.

### `OPENASSETIOTEST_ENABLE_SUBPROJECT=ON`

When `OPENASSETIOTEST_ENABLE_SUBPROJECT=ON`, the `OpenAssetIO` git
submodule under this repository must be populated. OpenAssetIO will then
be built as a subproject (`add_subdirectory(OpenAssetIO)`) and so all
its dependencies must be discoverable by CMake. See the OpenAssetIO
repository for more information.

> **Note**
>
> Python test targets are currently unsupported in this mode. If
> `OPENASSETIOTEST_ENABLE_PYTHON` is `ON` (default) then CMake will fail
> to configure with an error.
