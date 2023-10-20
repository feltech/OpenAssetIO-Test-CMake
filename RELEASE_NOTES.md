Release Notes
=============

v1.0.0-alpha.x
--------------

### Breaking Changes

* Update to use non-deprecated `TraitsData` path/namespace.
  [#13](https://github.com/OpenAssetIO/OpenAssetIO-Test-CMake/pull/13)

### New Features

* OpenAssetIO-MediaCreation integrated as a dependency, with tests.
[#7](https://github.com/OpenAssetIO/OpenAssetIO-Test-CMake/pull/7)

* CI testing added to exercise convergent dependency acquisition modes.
[#7](https://github.com/OpenAssetIO/OpenAssetIO-Test-CMake/pull/7)

### Bug Fixes

* Updated build configuration such that C API tests use the C++ linker,
  fixing issues where the C++ standard library is not found on the
  default C linker search paths.
  [#14](https://github.com/OpenAssetIO/OpenAssetIO-Test-CMake/pull/14)

v1.0.0-alpha.1
--------------

### New Features

* Initial version testing OpenAssetIO 1.0.0-alpha.6.
