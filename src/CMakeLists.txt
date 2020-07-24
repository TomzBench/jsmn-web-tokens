# jsmn_tokens

# install headers
set(INCLUDE_DIRECTORY ../include)
list(APPEND INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn/jsmn_tokens.h)
list(APPEND INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn/jsmn_helpers.h)
if(WITH_JSMN_HEADER_INSTALL)
  list(APPEND INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn/jsmn.h)
endif()

# source files
set(SOURCES
        crypto/base64.c
        crypto/base64uri.c
	jsmn_helpers.c
	jsmn_tokens.c)

# header files
set(HEADERS
	${INSTALL_HEADERS}
        crypto/base64.h
        crypto/base64uri.h)

list(APPEND DEFS "-DJSMN_MAX_TOKEN_LEN=${JSMN_MAX_TOKEN_LEN}")
list(APPEND DEFS "-DJSMN_MAX_TOKEN_HEADER_LEN=${JSMN_MAX_TOKEN_HEADER_LEN}")
list(APPEND DEFS "-DJSMN_MAX_TOKENS=${JSMN_MAX_TOKENS}")
list(APPEND INCS "${INCLUDE_DIRECTORY} ./")

if (${WITH_CRYPTO} STREQUAL OPENSSL)
  message(STATUS "Using OpenSSL")
  include(../cmake/openssl.cmake)
  set(CRYPTO_STATIC_LIBS ${OPENSSL_STATIC_LIBRARIES})
  set(CRYPTO_SHARED_LIBS ${OPENSSL_SHARED_LIBRARIES})
  list(APPEND SOURCES crypto/base64.c crypto/crypto_openssl.c)
  list(APPEND HEADERS crypto/base64.h crypto/crypto.h crypto/crypto_openssl.h)
  list(APPEND DEFS "-DWITH_OPENSSL")
  list(APPEND INCS "${OPENSSL_INCLUDE_DIR}")
elseif(${WITH_CRYPTO} STREQUAL GNUTLS)
  message(FATAL_ERROR "GNUTLS not support at this time")
elseif(${WITH_CRYPTO} STREQUAL MBEDTLS)
  message(STATUS "Using MBedTLS")
  include(../cmake/mbedtls.cmake)
  set(CRYPTO_STATIC_LIBS ${MBEDTLS_STATIC_LIBRARIES})
  set(CRYPTO_SHARED_LIBS ${MBEDTLS_SHARED_LIBRARIES})
  list(APPEND SOURCES crypto/crypto_mbedtls.c)
  list(APPEND HEADERS crypto/crypto.h crypto/crypto_mbedtls.h)
  list(APPEND DEFS "-DWITH_MBEDTLS")
elseif(${WITH_CRYPTO} STREQUAL WOLFSSL)
  message(FATAL_ERROR "WolfSSL Not supported at this time")
else()
  message(FATAL_ERROR 
    "Please select a supported crypto library
    [MBEDTLS, WOLFSSL, OPENSSL, GNUTLS]")
endif()

if(ENABLE_TESTING)
  add_library(jsmn_tokens_testing STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_testing ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_tokens_testing PUBLIC "${INCLUDE_DIRECTORY}")
  target_include_directories(jsmn_tokens_testing PUBLIC ./)
  target_compile_definitions(jsmn_tokens_testing PUBLIC ${DEFS})
  set_target_properties(jsmn_tokens_testing PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
  if(INCS)
    target_include_directories(jsmn_tokens_testing PUBLIC ${INCS})
  endif()
endif()

if(BUILD_SHARED)
  add_library(jsmn_tokens_shared SHARED ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_shared ${CRYPTO_SHARED_LIBS})
  target_include_directories(jsmn_tokens_shared PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_tokens_shared PRIVATE ./)
  target_compile_definitions(jsmn_tokens_shared PRIVATE ${DEFS})
  set_target_properties(jsmn_tokens_shared PROPERTIES
    PUBLIC_HEADER "${INSTALL_HEADERS}"
    COMPILE_DEFINITIONS "JSMN_TOKENS_EXPORT"
    OUTPUT_NAME jsmn-web-tokens)
  if(INCS)
    target_include_directories(jsmn_tokens_shared PUBLIC ${INCS})
  endif()
  install(TARGETS jsmn_tokens_shared
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          RUNTIME DESTINATION bin
          PUBLIC_HEADER DESTINATION include/jsmn)
endif()

if(BUILD_STATIC)
  add_library(jsmn_tokens_static STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_static ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_tokens_static PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_tokens_static PRIVATE ./)
  target_compile_definitions(jsmn_tokens_static PRIVATE ${DEFS})
  set_target_properties(jsmn_tokens_static PROPERTIES
    PUBLIC_HEADER "${INSTALL_HEADERS}"
    OUTPUT_NAME jsmn-web-tokens)
  if(INCS)
    target_include_directories(jsmn_tokens_static PUBLIC ${INCS})
  endif()
  install(TARGETS jsmn_tokens_static
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          RUNTIME DESTINATION bin
          PUBLIC_HEADER DESTINATION include/jsmn)
endif()
