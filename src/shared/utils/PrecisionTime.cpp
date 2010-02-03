/////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class wrapper for the system's high precision timer.
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "PrecisionTime.h"

// **************************************************************************
// Function:   Now()
// Purpose:    gets the current time from the system's high-performance timers
// Parameters:
// Returns:    an unsigned sixteen bit value for the current time in ms
//             (-> wrap-around occurs every 65536ms, or about 65 seconds)
// **************************************************************************
#if defined ( _WIN32 )
// **************************************************************************

#include <windows.h>
PrecisionTime
PrecisionTime::Now()
{
  // Get the current time from the Windows precision timer.
  LARGE_INTEGER prectime, prectimebase;
  ::QueryPerformanceCounter( &prectime );
  ::QueryPerformanceFrequency( &prectimebase );
  return ( prectime.QuadPart * 1000 ) / prectimebase.QuadPart;
}

// **************************************************************************
#elif defined ( __APPLE__ )
// **************************************************************************

#include <mach/mach_time.h>
PrecisionTime
PrecisionTime::Now()
{
  static int64_t mt0;
  static double  multiplier = 0.0;  
  if(!multiplier) {
  	mach_timebase_info_data_t mtbinfo;
    mach_timebase_info( &mtbinfo );
    multiplier = 1.0e-6 * (double(mtbinfo.numer) / double(mtbinfo.denom));
    mt0 = mach_absolute_time();
  }
  return multiplier * double(mach_absolute_time() - mt0);
}

// **************************************************************************
#else // neither _WIN32 nor __APPLE__
// **************************************************************************

#include <time.h>
PrecisionTime
PrecisionTime::Now()
{
  // Use clock_gettime() on non-Windows systems with librt.
  struct timespec t;
  ::clock_gettime( CLOCK_REALTIME, &t );
  return ( t.tv_sec * 1000 ) + t.tv_nsec / 1000000;
}

// **************************************************************************
#endif // _WIN32, __APPLE__
// **************************************************************************



// **************************************************************************
// Function:   TimeDiff
// Purpose:    calculates the difference between two times (i.e., time2-time1)
//             takes roll-over into account (in case time2 < time1)
// Parameters: time1, time2 - two 16 bit integers
// Returns:    time2-time1, if time2-time1 >= 0
//             or time2-time1+65536, if time2-time1 < 0
// **************************************************************************
PrecisionTime::NumType
PrecisionTime::TimeDiff( NumType time1, NumType time2 )
{
  const int maxdiffPlusOne = 1 << ( 8 * sizeof( time1 ) );
  return ( ( time2 + maxdiffPlusOne ) - time1 ) % maxdiffPlusOne;
}

// **************************************************************************
// Function:   SignedDiff
// Purpose:    calculates the signed difference between two times,
//             taking roll-over into account.
// Parameters: time1, time2 - two 16 bit integers
// Returns:    signed difference
// **************************************************************************
int
PrecisionTime::SignedDiff( NumType time1, NumType time2 )
{
  const int wraparound = 1 << ( 8 * sizeof( time1 ) );
  int diff = time1 - time2;
  if( diff >= wraparound / 2 )
    diff -= wraparound;
  else if( diff < -wraparound / 2 )
    diff += wraparound;
  return diff;
}

