# Copyright 2022 The Foundry Visionmongers Ltd
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)

# Set a default build type if none was specified.
# The CMake default is toolchain-specific so ensure consistency by
# having an explicit default.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui, ccmake
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "RelWithDebInfo")
endif ()

project(OpenAssetIO-Test-CMake)
enable_testing()

#-----------------------------------------------------------------------
# Options

option(OPENASSETIOTEST_ENABLE_SUBPROJECT "Use OpenAssetIO as a subproject" OFF)
option(OPENASSETIO_MEDIACREATIONTEST_ENABLE_SUBPROJECT
       "Use OpenAssetIO-MediaCreation as a subproject" OFF)
option(OPENASSETIOTEST_ENABLE_PYTHON "Test Python bindings" ON)
option(OPENASSETIOTEST_ENABLE_C "Test C bindings" ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 5.0)
    option(OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI "For gcc, use the new C++11 library ABI" OFF)
endif ()

if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file                = ${CMAKE_TOOLCHAIN_FILE}")
endif ()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "New C++11 ABI for GCC               = ${OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI}")
endif ()
message(STATUS "Testing OpenAssetIO as a subproject = ${OPENASSETIOTEST_ENABLE_OPENASSETIO_SUBPROJECT}")
message(STATUS "Testing OpenAssetIO-MediaCreation as a subproject = "
               "${OPENASSETIOTEST_ENABLE_MEDIACREATION_SUBPROJECT}")
message(STATUS "Testing OpenAssetIO Python bindings = ${OPENASSETIOTEST_ENABLE_PYTHON}")
message(STATUS "Testing OpenAssetIO C bindings      = ${OPENASSETIOTEST_ENABLE_C}")


#-----------------------------------------------------------------------
# OpenAssetIO dependency.

if (OPENASSETIOTEST_ENABLE_OPENASSETIO_SUBPROJECT)
    # Use OpenAssetIO as a subproject.
    #
    # This will add to the project using `add_subdirectory`.
    #
    # OpenAssetIO must therefore be built, meaning its `find_package`
    # calls for dependencies must work.

    add_subdirectory(OpenAssetIO)
else ()
    # Use OpenAssetIO as an external CMake package
    #
    # The `OpenAssetIOConfig.cmake` file must be discoverable. E.g.
    # add the install directory (`dist`) to CMAKE_PREFIX_PATH.

    find_package(OpenAssetIO REQUIRED)
endif ()

if (OPENASSETIOTEST_ENABLE_MEDIACREATION_SUBPROJECT)
    # Use OpenAssetIO-MediaCreation as a subproject.
    #
    # This will add to the project using `add_subdirectory`.

    add_subdirectory(OpenAssetIO-MediaCreation)
else ()
    # Use OpenAssetIO-MediaCreation as an external CMake package
    #
    # The `OpenAssetIO-MediaCreationConfig.cmake` file must be
    # discoverable. E.g. add the install directory (`dist`) to
    # CMAKE_PREFIX_PATH.

    find_package(OpenAssetIO-MediaCreation REQUIRED)
endif ()


# For Windows tests to work we must add the dlls to the PATH
if (WIN32 AND DEFINED OpenAssetIO_BINARY_DIR)
    file(TO_NATIVE_PATH "${OpenAssetIO_BINARY_DIR}" OpenAssetIO_BINARY_DIR_NATIVE)
endif ()

#-----------------------------------------------------------------------
# C++ OpenAssetIO tests.

add_executable(test.cpp.core src/test.core.cpp)
add_test(cpp.core test.cpp.core)
target_link_libraries(test.cpp.core PRIVATE OpenAssetIO::openassetio-core)
target_compile_features(test.cpp.core PRIVATE cxx_std_17)

# GCC's C++11 ABI toggle must match OpenAssetIO's settings.
if (DEFINED OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI)
    if (OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI)
        target_compile_definitions(test.cpp.core PRIVATE _GLIBCXX_USE_CXX11_ABI=1)
    else ()
        target_compile_definitions(test.cpp.core PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
    endif ()
endif ()

if (WIN32 AND DEFINED OpenAssetIO_BINARY_DIR)
    set_tests_properties(cpp.core PROPERTIES ENVIRONMENT "PATH=${OpenAssetIO_BINARY_DIR_NATIVE}")
endif ()

#-----------------------------------------------------------------------
# C++ MediaCreation tests.

add_executable(test.cpp.mediacreation src/test.mediacreation.cpp)
add_test(cpp.mediacreation test.cpp.mediacreation)
target_link_libraries(test.cpp.mediacreation PRIVATE OpenAssetIO::openassetio-core OpenAssetIO-MediaCreation::openassetio-mediacreation)
target_compile_features(test.cpp.mediacreation PRIVATE cxx_std_17)

# GCC's C++11 ABI toggle must match OpenAssetIO's settings.
if (DEFINED OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI)
    if (OPENASSETIOTEST_GLIBCXX_USE_CXX11_ABI)
        target_compile_definitions(test.cpp.mediacreation PRIVATE _GLIBCXX_USE_CXX11_ABI=1)
    else ()
        target_compile_definitions(test.cpp.mediacreation PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
    endif ()
endif ()

if (WIN32 AND DEFINED OpenAssetIO_BINARY_DIR)
    set_tests_properties(cpp.mediacreation PROPERTIES ENVIRONMENT "PATH=${OpenAssetIO_BINARY_DIR_NATIVE}")
endif ()


#-----------------------------------------------------------------------
# C tests.

if (OPENASSETIOTEST_ENABLE_C)
    add_executable(test.c.core src/test.core.c)
    add_test(c.core test.c.core)
    target_link_libraries(test.c.core PRIVATE OpenAssetIO::openassetio-core-c)
    # Must use C++ linker settings or the C++ standard library might not
    # be linked. This is required since openassetio-core-c depends on
    # openassetio-core, which is a C++ library.
    set_target_properties(test.c.core PROPERTIES LINKER_LANGUAGE CXX)
    target_compile_features(test.c.core PRIVATE c_std_99)
    if (WIN32 AND DEFINED OpenAssetIO_BINARY_DIR)
        set_tests_properties(c.core PROPERTIES ENVIRONMENT "PATH=${OpenAssetIO_BINARY_DIR_NATIVE}")
    endif ()
endif ()

#-----------------------------------------------------------------------
# Python tests.

if (OPENASSETIOTEST_ENABLE_PYTHON)

    if (OPENASSETIOTEST_ENABLE_SUBPROJECT)
        message(
            FATAL_ERROR
            "Testing OpenAssetIO as a subproject when Python is enabled is currently unsupported."
            " The binary Python module must be built and made available alongside the Python"
            " sources for tests to work."
        )
    endif ()

    find_package(Python COMPONENTS Interpreter Development.Embed REQUIRED)

    if (NOT ${Python_VERSION_MAJOR} EQUAL ${OpenAssetIO_Python_VERSION_MAJOR} OR
        NOT ${Python_VERSION_MINOR} EQUAL ${OpenAssetIO_Python_VERSION_MINOR})
        message(
            FATAL_ERROR
            "Found Python '${Python_VERSION}' but OpenAssetIO was built against"
            " '${OpenAssetIO_Python_VERSION}'"
        )
    endif ()

    #-----------------------------------------------------------------------
    # Python sources tests.

    # Python sources test.
    # -v - verbose output.
    # -B - don't write pycache files (ensure script is reinterpreted on
    # re-runs).
    # -s - ignore the user site-directory (avoid unexpected things
    # leaking in).
    add_test(NAME python COMMAND ${Python_EXECUTABLE} -vBs ${PROJECT_SOURCE_DIR}/src/test.py)
    set_tests_properties(python PROPERTIES ENVIRONMENT PYTHONPATH=${OpenAssetIO_Python_SITELIB})

    #-----------------------------------------------------------------------
    # Python bridge test (loading Python plugin system from C++).

    add_executable(test.python.bridge src/test.python.bridge.cpp)
    add_test(python.bridge.cpp test.python.bridge)
    target_link_libraries(
        test.python.bridge
        PRIVATE
        OpenAssetIO::openassetio-python-bridge
        Python::Python
    )
    target_compile_features(test.python.bridge PRIVATE cxx_std_17)
    # If libpython is linked in as a static library, then we must export
    # symbols for dynamically loaded Python extension modules to use.
    set_target_properties(test.python.bridge PROPERTIES ENABLE_EXPORTS ON)

    # Storage for list of environment variables to set before running
    # the test.
    set(_envvars)

    # Add location of Python sources in the OpenAssetIO package to the
    # PYTHONPATH.
    list(APPEND _envvars "PYTHONPATH=${OpenAssetIO_Python_SITELIB}")

    # The test has an embedded Python interpreter, but is missing all
    # the required basic Python libraries, so we need to set PYTHONHOME
    # to the source Python package to avoid nasty errors.
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import sys; sys.stdout.write(sys.prefix)"
        OUTPUT_VARIABLE Python_PREFIX
    )
    list(APPEND _envvars "PYTHONHOME=${Python_PREFIX}")

    # Fix library search path on MacOS.
    if (APPLE)
        # The `Python::Python` target's link options are set to e.g
        # `@rpath/Python.framework/Versions/3.9/Python` but all our
        # target's (CMake generated) @rpaths already contain
        # `Python.framework/Versions/3.9`. So add another RPATH with
        # the framework subdirectories stripped. This then allows our
        # test target to find the Python library.
        set_target_properties(test.python.bridge PROPERTIES BUILD_RPATH ${Python_PREFIX}/../../..)
    endif ()

    # Augment the PATH on Windows to find Python library dlls.
    if (WIN32)
        if (DEFINED OpenAssetIO_BINARY_DIR)
            list(APPEND _envvars.path ${OpenAssetIO_BINARY_DIR_NATIVE})
        endif ()
        foreach (path ${Python_RUNTIME_LIBRARY_DIRS})
            file(TO_NATIVE_PATH "${path}" path_native)
            list(APPEND _envvars.path ${path_native})
        endforeach ()
        # Frustratingly, $<SEMICOLON> doesn't work here, we have to find
        # the magic number of backslashes instead.
        list(JOIN _envvars.path \\\; _envvars.path)
        list(APPEND _envvars "PATH=${_envvars.path}")
    endif ()

    set_tests_properties(python.bridge.cpp PROPERTIES ENVIRONMENT "${_envvars}")
endif ()
