###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}BCPy2000/" )

OPTION( BUILD_BCPY2000 "Build BCPy2000 modules" OFF )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( foundation )
