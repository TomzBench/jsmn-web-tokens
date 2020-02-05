# jsmn_web_token

# install headers
set(INCLUDE_DIRECTORY ../include)
set(INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn_web_token/jsmn_web_token.h)

# source files
set(SOURCES
	jsmn_helpers.c
	jsmn_web_token.c)

# header files
set(HEADERS
	${INSTALL_HEADERS}
	jsmn.h
	jsmn_helpers.h
	jsmn_web_token_private.h)

if(ENABLE_TESTING)
  add_library(jsmn_web_token_testing STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_web_token_testing ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_web_token_testing PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_web_token_testing PUBLIC ./)
  set_target_properties(jsmn_web_token_testing PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()

if(BUILD_SHARED)
  add_library(jsmn_web_token_shared SHARED ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_web_token_testing ${CRYPTO_SHARED_LIBS})
  target_include_directories(jsmn_web_token_shared PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_web_token_shared PRIVATE ./)
  set_target_properties(jsmn_web_token_shared PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()

if(BUILD_STATIC)
  add_library(jsmn_web_token_static STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(jsmn_web_token_testing ${CRYPTO_STATIC_LIBS})
  target_include_directories(jsmn_web_token_static PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_web_token_static PRIVATE ./)
  set_target_properties(jsmn_web_token_static PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()
