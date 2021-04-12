# Get a list of log options
function (log_options result)
  list(APPEND _LOG_OPTIONS
    "TRACE" 
    "DEBUG" 
    "INFO" 
    "WARN" 
    "ERROR"
    "FATAL" 
    "NONE")
  set(${result} ${_LOG_OPTIONS} PARENT_SCOPE)
endfunction()

#
function (log_config prj log)
  list(APPEND _LOG_INTS 6 5 4 3 2 1 0)
  log_options(_LOG_OPTIONS)
  set(_LOG_DEFINITION "${prj}_LOG_LEVEL=0")
  foreach(LEVEL NUM  IN ZIP_LISTS _LOG_OPTIONS _LOG_INTS)
    if("${log}" STREQUAL "${LEVEL}")
      set(_LOG_DEFINITION "${prj}_LOG_LEVEL=${NUM}")
    endif()
  endforeach()
  set("${prj}_LOG_DEFINITIONS" "${_LOG_DEFINITION}" PARENT_SCOPE)
endfunction()
