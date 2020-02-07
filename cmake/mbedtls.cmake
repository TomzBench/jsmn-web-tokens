set(MBEDTLS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/mbedtls)
set(MBEDTLS_TEST_FILE ${MBEDTLS_SOURCE_DIR}/CMakeLists.txt)
set(MBEDTLS_CRYPTO_TEST_FILE ${MBEDTLS_SOURCE_DIR}/crypto/CMakeLists.txt)

if(WITH_SYSTEM_DEPENDENCIES)
  # referenced https://github.com/curl/curl/blob/master/CMake/FindMbedTLS.cmake
  find_path(MBEDTLS_INCLUDE_DIRS mbedtls/ssl.h)
  find_library(MBEDTLS_LIBRARY mbedtls)
  find_library(MBEDX509_LIBRARY mbedx509)
  find_library(MBEDCRYPTO_LIBRARY mbedcrypto)
  set(MBEDTLS_STATIC_LIBRARIES ${MBEDCRYPTO_LIBRARY} ${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY})
  set(MBEDTLS_SHARED_LIBRARIES ${MBEDCRYPTO_LIBRARY} ${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY})

else()
  set(MBEDTLS_STATIC_LIBRARIES crypto-static tls-static x509-static)
  set(MBEDTLS_SHARED_LIBRARIES crypto-shared tls-shared x509-shared)

  # clone mbedtls
  add_custom_command(
    OUTPUT ${MBEDTLS_TEST_FILE}
    COMMAND "${GIT_EXECUTABLE}" submodule update --init mbedtls
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  add_custom_target(mbedtls-repository DEPENDS "${MBEDTLS_TEST_FILE}")

  # clone mbedtls/crypto
  add_custom_command(
    OUTPUT ${MBEDTLS_CRYPTO_TEST_FILE}
    COMMAND "${GIT_EXECUTABLE}" submodule update --init crypto
    WORKING_DIRECTORY "${MBEDTLS_SOURCE_DIR}")
  add_custom_target(mbedtls-submodules DEPENDS "${MBEDTLS_CRYPTO_TEST_FILE}")
  add_dependencies(mbedtls-submodule mbedtls-repository)

endif()

if(NOT MSVC)
  ### Build Wolfssl ###
  ExternalProject_Add(mbedtls-project
  	SOURCE_DIR ${MBEDTLS_SOURCE_DIR}
  	INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        UPDATE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND 
            cmake
            --build . 
            --target install
            --config Release
        LIST_SEPARATOR |
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DENABLE_PROGRAMS:BOOL=OFF
            -DENABLE_TESTING:BOOL=OFF
            -DUSE_SHARED_MBEDTLS_LIBRARY:BOOL=ON
            -DUSE_STATIC_MBEDTLS_LIBRARY:BOOL=ON
  	EXCLUDE_FROM_ALL true)
  add_dependencies(mbedtls-project mbedtls-submodules)
  ExternalProject_Get_Property(mbedtls-project INSTALL_DIR)
  set(mbedtls_INCLUDE_DIR ${INSTALL_DIR}/include)
  FILE(MAKE_DIRECTORY ${INSTALL_DIR}/include)

  set(crypto_static_LIBRARY ${CMAKE_STATIC_LIBRARY_PREFIX}mbedcrypto${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(crypto_shared_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}mbedcrypto${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(crypto_static_LIBRARY ${INSTALL_DIR}/lib/${crypto_static_LIBRARY})
  set(crypto_shared_LIBRARY ${INSTALL_DIR}/lib/${crypto_shared_LIBRARY})

  set(tls_static_LIBRARY ${CMAKE_STATIC_LIBRARY_PREFIX}mbedtls${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(tls_shared_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}mbedtls${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(tls_static_LIBRARY ${INSTALL_DIR}/lib/${tls_static_LIBRARY})
  set(tls_shared_LIBRARY ${INSTALL_DIR}/lib/${tls_shared_LIBRARY})

  set(x509_static_LIBRARY ${CMAKE_STATIC_LIBRARY_PREFIX}mbedx509${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(x509_shared_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}mbedx509${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(x509_static_LIBRARY ${INSTALL_DIR}/lib/${x509_static_LIBRARY})
  set(x509_shared_LIBRARY ${INSTALL_DIR}/lib/${x509_shared_LIBRARY})
else()
endif()

# crypto-static
add_library(crypto-static STATIC IMPORTED)
set_property(TARGET crypto-static PROPERTY IMPORTED_LOCATION ${crypto_static_LIBRARY})
set_property(TARGET crypto-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(crypto-static mbedtls-project)

# crypto-shared
add_library(crypto-shared STATIC IMPORTED)
set_property(TARGET crypto-shared PROPERTY IMPORTED_LOCATION ${crypto_shared_LIBRARY})
set_property(TARGET crypto-shared PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(crypto-shared mbedtls-project)

# tls-static
add_library(tls-static STATIC IMPORTED)
set_property(TARGET tls-static PROPERTY IMPORTED_LOCATION ${tls_static_LIBRARY})
set_property(TARGET tls-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(tls-static mbedtls-project)

# tls-shared
add_library(tls-shared STATIC IMPORTED)
set_property(TARGET tls-shared PROPERTY IMPORTED_LOCATION ${tls_shared_LIBRARY})
set_property(TARGET tls-shared PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(tls-shared mbedtls-project)

# x509-static
add_library(x509-static STATIC IMPORTED)
set_property(TARGET x509-static PROPERTY IMPORTED_LOCATION ${x509_static_LIBRARY})
set_property(TARGET x509-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(x509-static mbedtls-project)

# x509-shared
add_library(x509-shared STATIC IMPORTED)
set_property(TARGET x509-shared PROPERTY IMPORTED_LOCATION ${x509_shared_LIBRARY})
set_property(TARGET x509-shared PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${mbedtls_INCLUDE_DIR})
add_dependencies(x509-shared mbedtls-project)
