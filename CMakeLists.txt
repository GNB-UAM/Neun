cmake_minimum_required(VERSION 3.5)

project(Neun VERSION 0.5.0 LANGUAGES CXX)

export(PACKAGE Neun)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Header-only library - define interface target
add_library(neun INTERFACE)
target_include_directories(neun INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/integrators>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/archetypes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/concepts>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/wrappers>
    $<INSTALL_INTERFACE:include/neun>
)
target_compile_features(neun INTERFACE cxx_std_20)

# Add tests (compile-time concept checks only)
add_subdirectory(tests)

# Installation for header-only library
install(DIRECTORY include/ integrators/ archetypes/ concepts/ models/ wrappers/
    DESTINATION include/neun
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS neun
    EXPORT neunTargets
    INCLUDES DESTINATION include/neun
)

install(EXPORT neunTargets
    FILE neunTargets.cmake
    NAMESPACE neun::
    DESTINATION lib/cmake/neun
)

# Create and install CMake config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/neunConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/neunConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/neunConfig.cmake"
    INSTALL_DESTINATION lib/cmake/neun
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/neunConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/neunConfigVersion.cmake"
    DESTINATION lib/cmake/neun
)

# Packaging
include(CPack)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An efficient library to simulate neural networks in C++.")
set(CPACK_PACKAGE_VENDOR "Grupo de Neurocomputación Biológica")
set(CPACK_PACKAGE_CONTACT "angel.lareo@uam.es")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Description")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_GENERATOR "DEB")

configure_file(neun.pc.in neun.pc @ONLY)
install(FILES neun.pc DESTINATION pkgconfig)
