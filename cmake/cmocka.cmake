# cmocka

set(CMOCKA_TRIPPLE "1.1.5")
set(CMOCKA_SOURCE_DIR "${EXTERNAL_DIR}/cmocka-${CMOCKA_TRIPPLE}")
set(CMOCKA_TEST_FILE "${CMOCKA_SOURCE_DIR}/README")
message(STATUS "FOO: ${DOWNLOAD_DIR}/cmocka-${CMOCKA_TRIPPLE}.tar.gz ${CMOCKA_TEST_FILE}")
check_extract("${DOWNLOAD_DIR}/cmocka-${CMOCKA_TRIPPLE}.tar.gz" "${CMOCKA_TEST_FILE}")

ExternalProject_Add(cmocka-project
	SOURCE_DIR "${CMOCKA_SOURCE_DIR}"
	INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
	UPDATE_COMMAND ""
	LIST_SEPARATOR |
	CMAKE_ARGS 
		-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
		-DCMAKE_INSTALL_LIBDIR=<INSTALL_DIR>/lib
		-DWITH_STATIC_LIB:BOOL=ON 
		-DUNIT_TESTING:BOOL=OFF
	)

ExternalProject_Get_Property(cmocka-project install_dir)
set(cmocka_LIBRARY ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cmocka-static${CMAKE_STATIC_LIBRARY_SUFFIX})
set(cmocka_INCLUDE_DIR ${install_dir}/include)

add_library(cmocka STATIC IMPORTED)
set_property(TARGET cmocka PROPERTY IMPORTED_LOCATION ${cmocka_LIBRARY})
set_property(TARGET cmocka PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${cmocka_INCLUDE_DIR})
add_dependencies(cmocka cmocka-project)
