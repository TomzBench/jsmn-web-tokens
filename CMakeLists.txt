cmake_minimum_required(VERSION 3.11...3.16)
project(jsmn_web_token)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)
# option(BUILD_EXAMPLES "Whether or not to build the examples" OFF)

if(ENABLE_TESTING)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_EXPORT_COMPILE_COMMANDS on)
  enable_testing()
  include(CTest)
  include(cmake/cmocka.cmake)
  include(cmake/CodeCoverage.cmake)
  add_subdirectory(test/jsmn)
endif()

if(BUILD_EXAMPLES)
endif()

add_subdirectory(src)
