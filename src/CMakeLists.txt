# jsmn_tokens

message(STATUS "JSMN_HEADER_FILE: ${JSMN_HEADER_FILE}")

# install headers
set(INCLUDE_DIRECTORY ../include)
list(APPEND INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn/jsmn_tokens.h)
if(WITH_JSMN_HEADER_INSTALL)
  list(APPEND INSTALL_HEADERS ${JSMN_HEADER_FILE})
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
        crypto/base64uri.h
	jsmn_helpers.h
	jsmn_tokens_private.h)

list(APPEND DEFS "-DJSMN_MAX_TOKEN_LEN=${JSMN_MAX_TOKEN_LEN}")
list(APPEND DEFS "-DJSMN_MAX_TOKEN_HEADER_LEN=${JSMN_MAX_TOKEN_HEADER_LEN}")
list(APPEND DEFS "-DJSMN_MAX_TOKENS=${JSMN_MAX_TOKENS}")
list(APPEND PUBDEFS "-DJSMN_HEADER_FILE=\"${JSMN_HEADER_FILE}\"")

if (WITH_OPENSSL)
  include(../cmake/openssl.cmake)
  set(CRYPTO_STATIC_LIBS ${OPENSSL_STATIC_LIBRARIES})
  set(CRYPTO_SHARED_LIBS ${OPENSSL_SHARED_LIBRARIES})
  list(APPEND SOURCES crypto/base64.c crypto/crypto_openssl.c)
  list(APPEND HEADERS crypto/base64.h crypto/crypto.h crypto/crypto_openssl.h)
  list(APPEND DEFS "-DWITH_OPENSSL")
elseif(WITH_GNUTLS)
  include(../cmake/gnutls.cmake)
elseif(WITH_MBEDTLS)
  include(../cmake/mbedtls.cmake)
  set(CRYPTO_STATIC_LIBS ${MBEDTLS_STATIC_LIBRARIES})
  set(CRYPTO_SHARED_LIBS ${MBEDTLS_SHARED_LIBRARIES})
  list(APPEND SOURCES crypto/crypto_mbedtls.c)
  list(APPEND HEADERS crypto/crypto.h crypto/crypto_mbedtls.h)
  list(APPEND DEFS "-DWITH_MBEDTLS")
elseif(WITH_WOLFSSL)
  include(../cmake/wolfssl.cmake)
else()
  message(FATAL_ERROR "Please select a crypto library")
endif()

if(ENABLE_TESTING)
  add_library(jsmn_tokens_testing STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_testing ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_tokens_testing PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_tokens_testing PUBLIC ./)
  target_compile_definitions(jsmn_tokens_testing PUBLIC ${DEFS})
  target_compile_definitions(jsmn_tokens_testing PUBLIC ${PUBDEFS})
  set_target_properties(jsmn_tokens_testing PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()

if(BUILD_SHARED)
  add_library(jsmn_tokens_shared SHARED ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_shared ${CRYPTO_SHARED_LIBS})
  target_include_directories(jsmn_tokens_shared PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_tokens_shared PRIVATE ./)
  target_compile_definitions(jsmn_tokens_shared PRIVATE ${DEFS})
  target_compile_definitions(jsmn_tokens_shared PUBLIC ${PUBDEFS})
  set_target_properties(jsmn_tokens_shared PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
  install(TARGETS jsmn_tokens_shared
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          PUBLIC_HEADER DESTINATION include/jsmn)
endif()

if(BUILD_STATIC)
  add_library(jsmn_tokens_static STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_tokens_static ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_tokens_static PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_tokens_static PRIVATE ./)
  target_compile_definitions(jsmn_tokens_static PRIVATE ${DEFS})
  target_compile_definitions(jsmn_tokens_static PUBLIC ${PUBDEFS})
  set_target_properties(jsmn_tokens_static PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
  install(TARGETS jsmn_tokens_static
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          PUBLIC_HEADER DESTINATION include/jsmn)
endif()
