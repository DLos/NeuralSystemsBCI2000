###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000Shell

# Set the executable name
SET( NAME BCI2000Shell )

UTILS_INCLUDE( frameworks/Core )

SET( SRC_PROJECT
  main.cpp
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
)

BCI2000_ADD_TARGET( EXECUTABLE ${NAME} ${SRC_PROJECT} )
SET_OUTPUT_DIRECTORY( "${BCI2000_ROOT_DIR}/prog" ${NAME} )
