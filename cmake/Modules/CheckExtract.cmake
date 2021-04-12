function (check_extract src dst test_file result)
  get_filename_component(REPO "${test_file}" DIRECTORY)
  if(NOT EXISTS "${test_file}")
    message(STATUS "${test_file} not found! extracting ${REPO}")
    execute_process(
    	COMMAND tar -xzvf "${src}" -C "${dst}"
        WORKING_DIRECTORY ${WORKSPACE_ROOT}
    	RESULT_VARIABLE EXTRACT_RESULT)
    set(${result} "${EXTRACT_RESULT}" PARENT_SCOPE)
  else()
    set(${result} 0 PARENT_SCOPE)
  endif()
endfunction()


