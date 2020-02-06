# --------------------------------------------------------------------------
# Build test-tokens
# --------------------------------------------------------------------------
add_executable(test-tokens main.c)
target_link_libraries(test-tokens
                      jsmn_tokens_testing
		      cmocka)

add_test(NAME test-tokens COMMAND test-tokens)
install(TARGETS test-tokens DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
