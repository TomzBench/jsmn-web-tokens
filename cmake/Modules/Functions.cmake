function (make_absolute path result)
  if(NOT IS_ABSOLUTE "${path}")
    get_filename_component(TMP "${path}" ABSOLUTE)
    set("${result}" "${TMP}" PARENT_SCOPE)
  endif()
endfunction()

function (resolve_install_dir loc result)
  set(TMP "${CMAKE_INSTALL_PREFIX}")
  if(NOT IS_ABSOLUTE "${TMP}")
    make_absolute("${TMP}" TMP)
  endif()
  set(TMP "${TMP}/${loc}")
  message(STATUS "RESOLVE ${loc} => ${TMP}")
  set("${result}" "${TMP}" PARENT_SCOPE)
endfunction()

function (copy_command src dst)
  add_custom_command(
    OUTPUT "${dst}"
    COMMAND ${CMAKE_COMMAND} -E copy
    "${src}"
    "${dst}"
  )
endfunction()

function (check_submodule root test_file)
  get_filename_component(REPO "${test_file}" DIRECTORY)
  if(NOT EXISTS "${test_file}")
    message(STATUS "${test_file} not found! cloneing submodule ${REPO}")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init "${REPO}"
      WORKING_DIRECTORY ${root}
      RESULT_VARIABLE R)
    message(STATUS "GIT SUBMODULE UPDATE RESULT: ${R}")
  else()
    message(STATUS "${test_file} found!")
  endif()
endfunction()

function (check_ruby gemfile)
  get_filename_component(REPO "${gemfile}" DIRECTORY)
  set(BUNDLE_CONFIG "${ROOT_DIR}/.bundle/vendor")
  file(WRITE "${REPO}/.bundle/config" "---\nBUNDLE_PATH: \"${BUNDLE_CONFIG}\"")
  execute_process(
    COMMAND bundle config --local path "${ROOT_DIR}/.bundle/vendor"
    WORKING_DIRECTORY "${ROOT_DIR}"
    RESULT_VARIABLE R)
  message(STATUS "Ruby config set local path result: ${R}")
  execute_process(
    COMMAND bundle install --gemfile "${gemfile}"
    WORKING_DIRECTORY "${ROOT_DIR}"
    RESULT_VARIABLE R)
  message(STATUS "Ruby Install Result: ${R}")
endfunction()

function (check_extract dl test_file)
  get_filename_component(REPO "${test_file}" DIRECTORY)
  if(NOT EXISTS "${test_file}")
    message(STATUS "${test_file} not found! extracting ${REPO}")
    execute_process(
    	COMMAND tar -xzvf "${dl}" -C "${EXTERNAL_DIR}"
    	WORKING_DIRECTORY ${ROOT_DIR}
    	RESULT_VARIABLE EXTRACT_RESULT)
    message(STATUS "EXTRACT_RESULT: ${EXTRACT_RESULT}")
  else()
    message(STATUS "${test_file} found!")
  endif()
endfunction()

function (check_extract_ex dl test_file transform)
  get_filename_component(REPO "${test_file}" DIRECTORY)
  if(NOT EXISTS "${test_file}")
    message(STATUS "${test_file} not found!")
    execute_process(
        COMMAND tar -xzvf "${dl}" -C "${REPO}" --transform "${transform}"
    	WORKING_DIRECTORY ${ROOT_DIR}
        COMMAND_ECHO STDOUT
    	RESULT_VARIABLE EXTRACT_RESULT)
    message(STATUS "EXTRACT_RESULT: ${EXTRACT_RESULT}")
  else()
    message(STATUS "${test_file} found!")
  endif()
endfunction()
