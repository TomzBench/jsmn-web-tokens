
set(DEFAULT_INSTALL "${CMAKE_BINARY_DIR}/install")
get_filename_component(DEFAULT_INSTALL "${DEFAULT_INSTALL}" ABSOLUTE)

function (normalizeInstallPrefix)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX 
      "${DEFAULT_INSTALL}" 
      CACHE PATH "Installation directory (default: ${DEFAULT_INSTAL})" 
      FORCE)
  else()
    get_filename_component(
      CMAKE_INSTALL_PREFIX 
      "${CMAKE_INSTALL_PREFIX}" 
      ABSOLUTE)
  endif()
  message(STATUS "INSTALL PREFIX: ${CMAKE_INSTALL_PREFIX}")
endfunction()
