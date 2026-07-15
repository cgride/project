cmake_minimum_required(VERSION 3.22)

project(
  cgride-project
  VERSION 0.1.0
  DESCRIPTION "C++ project model with targets, sources, includes and links for Cgride."
  HOMEPAGE_URL "https://github.com/cgride/project"
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CGRIDE_PROJECT_BUILD_TESTS "Build Cgride project tests" ON)
option(CGRIDE_PROJECT_INSTALL "Install Cgride project" ON)

if(NOT TARGET cgride::core)
  find_package(cgride-core CONFIG REQUIRED)
endif()

add_library(cgride_project
  src/source_set.cpp
  src/requirement.cpp
  src/target.cpp
  src/project.cpp
  src/validation.cpp
)

add_library(cgride::project ALIAS cgride_project)

target_compile_features(cgride_project
  PUBLIC
    cxx_std_23
)

target_include_directories(cgride_project
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cgride_project
  PUBLIC
    cgride::core
)

set_target_properties(cgride_project
  PROPERTIES
    OUTPUT_NAME cgride_project
    EXPORT_NAME project
    CXX_EXTENSIONS OFF
)

if(CGRIDE_PROJECT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CGRIDE_PROJECT_INSTALL)
  install(
    TARGETS cgride_project
    EXPORT cgride-project-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT cgride-project-targets
    NAMESPACE cgride::
    FILE cgride-project-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-project
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cgride-project-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-project-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-project
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-project-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-project-config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-project-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-project
  )
endif()
