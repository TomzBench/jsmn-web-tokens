# jsmn_wt

# install headers
set(INCLUDE_DIRECTORY ../include)
set(INSTALL_HEADERS ${INCLUDE_DIRECTORY}/jsmn_wt/jsmn_wt.h)

# source files
set(SOURCES
	jsmn_helpers.c
	jsmn_wt.c)

# header files
set(HEADERS
	${INSTALL_HEADERS}
	jsmn.h
	jsmn_helpers.h
	jsmn_wt_private.h)

# TODO select ssl library
if (WITH_OPENSSL)
elseif(WITH_GNU_TLS)
elseif(WITH_MBEDTLS)
elseif(WITH_WOLFSSL)
else()
endif()

if(ENABLE_TESTING)
  add_library(jsmn_wt_testing STATIC ${SOURCES} ${HEADERS})
  target_include_directories(jsmn_wt_testing PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_wt_testing PUBLIC ./)
  set_target_properties(jsmn_wt_testing PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()

if(BUILD_SHARED)
  add_library(jsmn_wt_shared SHARED ${SOURCES} ${HEADERS})
  target_include_directories(jsmn_wt_shared PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_wt_shared PRIVATE ./)
  set_target_properties(jsmn_wt_shared PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()

if(BUILD_STATIC)
  add_library(jsmn_wt_static STATIC ${SOURCES} ${HEADERS})
  target_include_directories(jsmn_wt_static PUBLIC ${INCLUDE_DIRECTORY})
  target_include_directories(jsmn_wt_static PRIVATE ./)
  set_target_properties(jsmn_wt_static PROPERTIES PUBLIC_HEADER "${INSTALL_HEADERS}")
endif()
