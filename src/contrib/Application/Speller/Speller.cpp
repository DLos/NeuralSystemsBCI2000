/* (C) 2000-2010, BCI2000 Project
/* http://www.bci2000.org
/*/
#include "PCHIncludes.h"
#pragma hdrstop

#include <vcl.h>
#include "CoreModuleVCL.h"

WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  try
  {
    Application->Initialize();
    Application->Title = "Speller Application";
    CoreModuleVCL().Run( _argc, _argv );
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}


