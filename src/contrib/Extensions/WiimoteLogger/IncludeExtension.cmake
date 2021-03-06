###########################################################################
## $Id$
## Authors: jezhill@gmail.com, griffin.milsap@gmail.com

IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  IF( MSVC )
    SET( WINDDK_LIB_DIR ${BCI2000_EXTENSION_DIR}/extlib/winddk/lib/msvc )
    SET( BCI2000_SIGSRCLIBS
       ${BCI2000_SIGSRCLIBS}
       "${WINDDK_LIB_DIR}/hid.lib"
       "${WINDDK_LIB_DIR}/setupapi.lib"
    )
  ENDIF()
ENDIF()

IF( NOT WINDDK_LIB_DIR )
  MESSAGE( "**** WiimoteLogger failed: hid and setupapi libraries are not found for this platform/compiler" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRCLIBDIRS
   ${BCI2000_SIGSRCLIBDIRS}
   ${WINDDK_LIB_DIR}
)

SET( BCI2000_SIGSRCINCDIRS
   ${BCI2000_SIGSRCINCDIRS}
   ${BCI2000_EXTENSION_DIR}/extlib/winddk/inc
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself
   ${BCI2000_EXTENSION_DIR}
)

SET( BCI2000_SIGSRCHEADERS_EXTENSIONS
   ${BCI2000_SIGSRCHEADERS_EXTENSIONS}
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote.h
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote_state.h
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote_common.h
   ${BCI2000_EXTENSION_DIR}/WiimoteLogger.h
)

SET( BCI2000_SIGSRCSOURCES_EXTENSIONS
   ${BCI2000_SIGSRCSOURCES_EXTENSIONS}
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote.cpp
   ${BCI2000_EXTENSION_DIR}/WiimoteLogger.cpp
)
