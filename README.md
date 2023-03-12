# OpenAssetIO Test Host

A bare-bones C++ host application with [OpenAssetIO](https://github.com/OpenAssetIO/OpenAssetIO)
as a dependency, for testing basic integration (linking, headers, etc).

Test targets are created that simply check that C++, C and
Python components can be imported.

> **Note**
>
> This project is intended for use in CI and not used as a reference.

## Getting started

This application can link against its dependent libraries, OpenAssetIO
and OpenAssetIO-MediaCreation in two modes, depending on
the value of the `OPENASSETIOTEST_ENABLE_OPENASSETIO_SUBPROJECT` and
`OPENASSETIOTEST_ENABLE_MEDIACREATION_SUBPROJECT` CMake variables
respectively - see below.

The usual CMake steps can be used to build and run the tests. Assuming
the current working directory is at the root of the repository:

```shell
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

### Dependency Modes

This project has two main dependencies, `OpenAssetIO` and
`OpenAssetIO-MediaCreation`. They can be discovered in two ways.

#### CMake Packages

When either `OPENASSETIOTEST_ENABLE_OPENASSETIO_SUBPROJECT` or
`OPENASSETIOTEST_ENABLE_MEDIACREATION_SUBPROJECT` are set to `OFF`
(which is the default if unspecified), the package will be discovered as
a CMake package (`find_package(...)`) and so the package must be
pre-built and installed into one of
[find_package](https://cmake.org/cmake/help/v3.21/command/find_package.html)'s
configured search paths.

#### Subprojects

When either `OPENASSETIOTEST_ENABLE_OPENASSETIO_SUBPROJECT` or
`OPENASSETIOTEST_ENABLE_MEDIACREATION_SUBPROJECT`are set to `ON`, the
package git submodule under this repository must be populated. The
package will then be built as a subproject (`add_subdirectory(...)`) and
so all its dependencies must be discoverable by CMake. See the
OpenAssetIO and OpenAssetIO-MediaCreation repositories for more
information.

> **Note**
>
> Python test targets are currently unsupported in this mode. If
> `OPENASSETIOTEST_ENABLE_PYTHON` is `ON` (default) then CMake will fail
> to configure with an error.

## Development notes

This package follows the main
[OpenAssetIO contribution process](https://github.com/OpenAssetIO/OpenAssetIO/blob/main/contributing/PROCESS.md).
