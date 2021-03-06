###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for NIDAQ-MX source module

# Set the executable name
SET( EXECUTABLE_NAME NIDAQ_mx_Source )

# Set the project specific sources
SET( SRC_PROJECT
  NIADC.cpp
  NIDAQ-Logging/NIDAQFilter.cpp
  NIDAQ-Logging/NIDAQLogger.cpp
  nidaq/NIDAQmx.imports.cpp
)
SET( HDR_PROJECT
  NIDAQmxADC.h
  nidaq/NIDAQmx.h
  nidaq/NIDAQmx.imports.h
  NIDAQ-Logging/NIDAQFilter.h
  NIDAQ-Logging/NIDAQLogger.h
)

INCLUDE_DIRECTORIES( ${BCI2000_SRC_DIR}/contrib/SignalSource/NIDAQ-MX/nidaq )
IF( USE_DYNAMIC_IMPORTS )
ELSE()
  LINK_DIRECTORIES( ${BCI2000_SRC_DIR}/contrib/SignalSource/NIDAQ-MX/nidaq )
ENDIF()

IF( USE_DYNAMIC_IMPORTS OR MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )
  IF( USE_DYNAMIC_IMPORTS )
  ELSEIF( MSVC )
    TARGET_LINK_LIBRARIES( "${EXECUTABLE_NAME}" NIDAQmx_orig.lib )
  ENDIF()
ELSE()
  MESSAGE( "NIDAQ-MX libraries not available for this compiler/OS, NIDAQ-MX source will not build." )
ENDIF()

