# Try to find the CDD library
#
# In order:
# - try to find_package(cdd) (upstream package)
# - try to use cdd-config to find the geos installation prefix
# - try to find the cdd library
#
# If the library is found, then you can use the cdd::cdd target
#
# This module will use CDD_PREFIX as an hint to find either the cdd-config
# executable or the cdd library

if(NOT TARGET cdd::cdd)
find_package(cdd QUIET)
if(NOT ${cdd_FOUND})
  if(NOT DEFINED CDD_PREFIX)
    set(CDD_PREFIX ${CMAKE_INSTALL_PREFIX})
  endif()
  # Find the CDD-config program
  find_program(CDD_CONFIG cdd-config
    /usr/local/bin
    /usr/bin
    ${CDD_PREFIX}/bin
    ${CMAKE_INSTALL_PREFIX}/bin
  )
  if(CDD_CONFIG)
    # Get CDD_INSTALL_PREFIX from cdd-config
    exec_program(${CDD_CONFIG} ARGS --prefix OUTPUT_VARIABLE CDD_INSTALL_PREFIX)
    find_library(CDD_LIBRARY
      NAME libcdd.so
      HINTS ${CDD_INSTALL_PREFIX}
    )
    if(NOT CDD_LIBRARY)
	    message(FATAL_ERROR "Found CDD install prefix (${CDD_INSTALL_PREFIX}) but no cdd library")
    endif()
    message("-- Found CDD by cdd-config: ${CDD_LIBRARY}")
  else()
      find_library(CDD_LIBRARY
      NAMES cdd 
      HINTS ${CDD_PREFIX} ${CMAKE_INSTALL_PREFIX}
      )
      find_path(CDD_INCLUDE_DIR
      NAMES cdd.h cddmp.h
      PATHS ${CDD_PREFIX}
    	    ${CDD_INSTALL_PREFIX}/include
      PATH_SUFFIXES include/cdd include/cddlib
      )
    if(CDD_LIBRARY)
      get_filename_component(CDD_INSTALL_PREFIX ${CDD_LIBRARY} DIRECTORY)
      set(CDD_INSTALL_PREFIX "${CDD_INSTALL_PREFIX}/..")
      get_filename_component(CDD_INSTALL_PREFIX ${CDD_INSTALL_PREFIX} ABSOLUTE)
      message("-- Found CDD library: ${CDD_LIBRARY}")
    else()
	    message(FATAL_ERROR "Could not find the cdd package, cdd-config or the cdd library, either you are missing the dependency or you should provide the CDD_PREFIX hint")
    endif()
  endif()
  add_library(cdd::cdd INTERFACE IMPORTED)
  set_target_properties(cdd::cdd PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CDD_INCLUDE_DIR} 
    #${CDD_INSTALL_PREFIX}/include
    INTERFACE_LINK_LIBRARIES ${CDD_LIBRARY}
  )
endif()
endif()
