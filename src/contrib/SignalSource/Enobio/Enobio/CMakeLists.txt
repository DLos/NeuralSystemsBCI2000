###########################################################################
## $Id$
## Authors: masayo.haneda@starlab.es
## Description: Build information for ENOBIO

# Set the executable name
SET( EXECUTABLE_NAME ENOBIO )

# Set the project specific sources
SET( SRC_PROJECT
  ENOBIOADC.cpp 
  lib/EnobioDLL.imports.cpp
)
SET( HDR_PROJECT
  ENOBIOADC.h
  lib/EnobioDLL.h
  lib/EnobioDLL.imports.h
)

IF( USE_DYNAMIC_IMPORTS )
ELSEIF( MSVC )
  LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC )
ENDIF()

# Put everything together
IF( NOT USE_DYNAMIC_IMPORTS AND (NOT MSVC OR CMAKE_SIZEOF_VOID_P EQUAL 8) )
  MESSAGE( "-- ENOBIO does not provide libraries for this compiler/OS,  Will not build" )
ELSE()

  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )

IF( NOT USE_DYNAMIC_IMPORTS )
  # Link the Enobio Library
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} EnobioDLL )
ENDIF()

  ADD_CUSTOM_COMMAND(
    TARGET "${EXECUTABLE_NAME}"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/EnobioDLL.dll" "${BCI2000_ROOT_DIR}/prog"
  )

ENDIF()


