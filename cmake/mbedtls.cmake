set(mbedtls_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/mbedtls)

if(NOT MSVC)
  ### Build Wolfssl ###
  ExternalProject_Add(mbedtls-project
  	SOURCE_DIR ${mbedtls_SOURCE_DIR}
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
