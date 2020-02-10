cmake_minimum_required(VERSION 3.11...3.16)
project(jsmn_tokens)

find_package(Git)
if(NOT Git_FOUND)
  message(FATAL_ERROR "Git not found! Git is required to install submodules")
endif()

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_POSITION_INDEPENDENT_CODE on)

# BOOL OPTIONS
option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)
option(WITH_SYSTEM_DEPENDENCIES "Do not build dependencies" OFF)
option(WITH_JSMN_HEADER_INSTALL "Install the jasmine header" ON)

# OPTIONS
set(JSMN_MAX_TOKEN_LEN 256 CACHE STRING "Largest token supported")
set(JSMN_MAX_TOKEN_HEADER_LEN 64 CACHE STRING "Largest token header supported")
set(JSMN_MAX_TOKENS 64 CACHE STRING "Largest decoded token supported")
set(WITH_CRYPTO "MBEDTLS" 
  CACHE STRING "Select crypto library [MBEDTLS, WOLFSSL, OPENSSL, GNUTLS]")

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
    "${PROJECT_SOURCE_DIR}/src/crypto/*"
    "${PROJECT_SOURCE_DIR}/include/jsmn/jsmn.h"
    "${PROJECT_SOURCE_DIR}/external"
    "${PROJECT_SOURCE_DIR}/examples"
    "${PROJECT_SOURCE_DIR}/test")

  setup_target_for_coverage_lcov(NAME test-coverage
    EXECUTABLE test-jsmn
    COMMAND test-tokens)

endif()

if(BUILD_EXAMPLES)
endif()

add_subdirectory(src)
