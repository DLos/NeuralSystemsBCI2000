////////////////////////////////////////////////////////////////////////////////
// $Id$
// File:        NotchFilter.h
// Description: A notch filter for removing power line noise.
// $Log$
// Revision 1.2  2006/05/04 17:17:01  mellinger
// Changed value for passband ripples.
//
// Revision 1.1  2006/05/04 17:06:43  mellinger
// Initial revision.
//
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "NotchFilter.h"
#include "FilterDesign.h"

using namespace std;

RegisterFilter( NotchFilter, 0.1 );

NotchFilter::NotchFilter()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source int NotchFilter= 0 0 0 2 "
      "// Power line notch filter: 0 disabled, 1: at 50 Hz, 2: at 60 Hz (enumeration)",
  END_PARAMETER_DEFINITIONS
}

void
NotchFilter::DesignFilter( num_seq_type& inputCoeff,
                           num_seq_type& outputCoeff ) const
{
  inputCoeff.clear();
  outputCoeff.clear();

  enum
  {
    disabled = 0,
    at50Hz = 1,
    at60Hz = 2,
  };
  int notchFilter = Parameter( "NotchFilter" );
  num_type corner1 = 0.0,
           corner2 = 0.0;

  switch( notchFilter )
  {
    case disabled:
      break;

    case at50Hz:
      corner1 = 45.0;
      corner2 = 55.0;
      break;

    case at60Hz:
      corner1 = 55.0;
      corner2 = 65.0;
      break;

    default:
      bcierr << "Unknown value of NotchFilter parameter" << endl;
  }

  if( notchFilter != disabled )
  {
    corner1 /= Parameter( "SamplingRate" );
    corner2 /= Parameter( "SamplingRate" );
    if( corner1 >= 0.5 || corner2 >= 0.5 )
    {
      bciout << "Power line frequency is outside sampling bandwidth. "
             << "No filtering will be performed"
             << endl;
    }
    else
    {
      Ratpoly<FilterDesign::Complex> tf =
        FilterDesign::Chebyshev()
        .Ripple_dB( -0.1 )
        .Order( 4 )
        .Bandstop( corner1, corner2 )
        .TransferFunction();
      num_type dcGain = abs( tf.Evaluate( 1.0 ) );

      FilterDesign::ComputeCoefficients( tf, inputCoeff, outputCoeff );
      for( size_t i = 0; i < inputCoeff.size(); ++i )
        inputCoeff[ i ] /= dcGain;
    }
  }
}
