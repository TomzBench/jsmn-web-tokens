cmake_minimum_required(VERSION 3.11...3.16)
project(jsmn_web_token)

find_package(Git)
if(NOT Git_FOUND)
  message(FATAL_ERROR "Git not found! Git is required to install submodules")
endif()

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)
# option(BUILD_EXAMPLES "Whether or not to build the examples" OFF)
option(WITH_OPENSSL "Link crypto primatives against openssl" OFF)
option(WITH_GNUTLS "Link crypto primatives against openssl" OFF)
option(WITH_MBEDTLS "Link crypto primatives against openssl" ON)
option(WITH_WOLFSSL "Link crypto primatives against openssl" OFF)

if(ENABLE_TESTING)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_EXPORT_COMPILE_COMMANDS on)
  enable_testing()
  include(CTest)
  include(cmake/cmocka.cmake)
  include(cmake/CodeCoverage.cmake)
  add_subdirectory(test/jsmn)
  add_subdirectory(test/tokens)

  APPEND_COVERAGE_COMPILER_FLAGS()
  set(COVERAGE_LCOV_EXCLUDES
    "${PROJECT_SOURCE_DIR}/examples"
    "${PROJECT_SOURCE_DIR}/test")

  setup_target_for_coverage_lcov(NAME test-coverage
    EXECUTABLE test-jsmn
    COMMAND test-tokens)

endif()

if(BUILD_EXAMPLES)
endif()

add_subdirectory(src)
