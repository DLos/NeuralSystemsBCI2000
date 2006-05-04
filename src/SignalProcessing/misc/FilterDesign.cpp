/////////////////////////////////////////////////////////////////////////////////
// $Id$
// File: FilterDesign.cpp
// Description: IIR filter design classes that compute coefficents for
//       Butterworth, Chebyshev, or Resonator type digital filters.
//       Based on mkfilter.C written by
//         A.J. Fisher, University of York   <fisher@minster.york.ac.uk>
//         September 1992
// $Log$
// Revision 1.1  2006/05/04 17:06:43  mellinger
// Initial revision.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma hdrstop

#include "FilterDesign.h"

#include <complex>
#include <cmath>
#include <limits>
#include <algorithm>
#include "UBCIError.h"

using namespace std;

// Math helper functions.
template<class T>
static
T sqr( const T& x )
{
  return x * x;
}

template<class T>
static
T asinh( const T& x )
{
  return ::log( x + ::sqrt( 1.0 + sqr( x ) ) );
}

namespace FilterDesign
{
// Butterworth type filter design
Butterworth::Butterworth()
: mOrder( 0 ),
  mCorner1( 0 ),
  mCorner2( 0 ),
  mCharacter( none )
{
}

Butterworth&
Butterworth::Order( int order )
{
  mOrder = order;
  return *this;
}

Butterworth&
Butterworth::Lowpass( Real corner )
{
  mCorner1 = mCorner2 = Prewarp( corner );
  mCharacter = lowpass;
  return *this;
}

Butterworth&
Butterworth::Highpass( Real corner )
{
  mCorner1 = mCorner2 = Prewarp( corner );
  mCharacter = highpass;
  return *this;
}

Butterworth&
Butterworth::Bandpass( Real corner1, Real corner2 )
{
  mCorner1 = Prewarp( corner1 );
  mCorner2 = Prewarp( corner2 );
  mCharacter = bandpass;
  return *this;
}

Butterworth&
Butterworth::Bandstop( Real corner1, Real corner2 )
{
  mCorner1 = Prewarp( corner1 );
  mCorner2 = Prewarp( corner2 );
  mCharacter = bandstop;
  return *this;
}

Real
Butterworth::Prewarp( Real a )
{
  return ::tan( M_PI * a ) / M_PI;
}

void
Butterworth::BilinearTransform( ListOfComplex& v )
{
  for( size_t i = 0; i < v.size(); ++i )
    v[ i ] = ( 2. + v[ i ] ) / ( 2. - v[ i ] );
}

/* compute S-plane poles for prototype LP filter */
void
Butterworth::S_Poles( ListOfComplex& outPoles ) const
{
  outPoles.clear();
  for( int i = 0; i < 2 * mOrder; ++i )
  {
    Real theta = ( mOrder & 1 ) ?
                   ( i * M_PI ) / mOrder :
                   ( ( i + 0.5 ) * M_PI ) / mOrder;
    Complex z = polar( 1.0, theta );
    if( z.real() < 0.0 )
      outPoles.push_back( z );
  }
}

void
Butterworth::Normalize( ListOfComplex& ioPoles,
                        ListOfComplex& outZeros ) const
{
  Real w1 = 2.0 * M_PI * mCorner1,
       w2 = 2.0 * M_PI * mCorner2;

  bool badFreq = false;
  switch( mCharacter )
  {
    case bandpass:
    case bandstop:
      if( w1 > w2 )
        swap( w1, w2 );
      badFreq = ( mCorner2 <= 0 || mCorner2 >= 0.5 );
      /* no break */
    case lowpass:
    case highpass:
      badFreq = badFreq || ( mCorner1 <= 0 || mCorner1 >= 0.5 );
      break;
  }
  if( badFreq )
  {
    bcierr << "Corner frequencies must be > 0 and < 0.5" << endl;
    return;
  }

  outZeros.clear();
  switch( mCharacter )
  {
    case lowpass:
      for( size_t i = 0; i < ioPoles.size(); ++i )
        ioPoles[ i ] *= w1;
      break;

    case highpass:
      for( size_t i = 0; i < ioPoles.size(); ++i )
        ioPoles[ i ] = w1 / ioPoles[ i ];
      outZeros.resize( ioPoles.size(), 0.0 );
      break;

    case bandpass:
      {
       Real w0 = sqrt( w1 * w2 ),
            bw = w2 - w1;
       ListOfComplex newPoles;
       for( size_t i = 0; i < ioPoles.size(); ++i )
       {
         Complex hba = 0.5 * ( ioPoles[ i ] * bw ),
                 temp = sqrt( 1.0 - sqr( w0 / hba ) );
         ioPoles[ i ] = hba * ( 1.0 + temp );
         newPoles.push_back( hba * ( 1.0 - temp ) );
       }
       outZeros.resize( ioPoles.size(), 0.0 );
       ioPoles.insert( ioPoles.end(), newPoles.begin(), newPoles.end() );
      }
      break;

    case bandstop:
      {
        Real w0 = sqrt( w1 * w2 ),
             bw = w2 - w1;
        ListOfComplex newPoles;
        for( size_t i = 0; i < ioPoles.size(); ++i )
        {
          Complex hba = 0.5 * ( bw / ioPoles[ i ] ),
                  temp = sqrt( 1.0 - sqr( w0 / hba ) );
          ioPoles[ i ] = hba * ( 1.0 + temp );
          newPoles.push_back( hba * ( 1.0 - temp ) );
        }
        for( size_t i = 0; i < ioPoles.size(); ++i )  /* also 2N zeros at (0, +-w0) */
        {
          outZeros.push_back( Complex( 0.0, +w0 ) );
          outZeros.push_back( Complex( 0.0, -w0 ) );
        }
        ioPoles.insert( ioPoles.end(), newPoles.begin(), newPoles.end() );
      }
      break;

    default:
      bcierr << "Unsupported filter character" << endl;
  }
}

Ratpoly<Complex>
Butterworth::TransferFunction() const
{
  ListOfComplex poles, zeros;
  S_Poles( poles );
  Normalize( poles, zeros );
  /* given S-plane poles & zeros, compute Z-plane poles & zeros, by bilinear transform */
  BilinearTransform( poles );
  BilinearTransform( zeros );
  if( zeros.size() < poles.size() )
    zeros.resize( poles.size(), -1 );
  return Ratpoly<Complex>(
           Polynomial<Complex>::FromRoots( zeros ),
           Polynomial<Complex>::FromRoots( poles )
         );
}

// Chebyshev type filter design is expressed as a modification of a Butterworth
// filter
Chebyshev::Chebyshev()
: mRipple_dB( 0 )
{
}

Chebyshev&
Chebyshev::Ripple_dB( Real ripple_dB )
{
  mRipple_dB = ripple_dB;
  return *this;
}

void
Chebyshev::S_Poles( ListOfComplex& outPoles ) const
{
  Butterworth::S_Poles( outPoles );
  if( mRipple_dB >= 0 )
    bcierr << "Ripple_dB parameter must be less than 0" << endl;

  /* modify poles for Chebyshev (p. 136 DeFatta et al.) */
  Real rip = ::pow( 10.0, -mRipple_dB / 10.0 ),
       eps = ::sqrt( rip - 1.0 ),
       y = asinh( 1.0 / eps ) / mOrder;
  if( y <= 0.0 )
    bcierr << "Bad ripple argument" << endl;
  for( size_t i = 0; i < outPoles.size(); ++i )
    outPoles[ i ] = Complex(
                      outPoles[ i ].real() * sinh( y ),
                      outPoles[ i ].imag() * cosh( y )
                    );
}

// 2-pole resonator filter design
Resonator::Resonator()
: mCenterFreq( 0 ),
  mQFactor( 1 ),
  mCharacter( none )
{
}

Resonator&
Resonator::QFactor( Real qFactor )
{
  mQFactor = qFactor;
  return *this;
}

Resonator&
Resonator::Bandpass( Real centerFreq )
{
  mCenterFreq = centerFreq;
  mCharacter = bandpass;
  return *this;
}

Resonator&
Resonator::Bandstop( Real centerFreq )
{
  mCenterFreq = centerFreq;
  mCharacter = bandstop;
  return *this;
}

Resonator&
Resonator::Allpass( Real centerFreq )
{
  mCenterFreq = centerFreq;
  mCharacter = allpass;
  return *this;
}

Ratpoly<Complex>
Resonator::TransferFunction() const
{
  bool paramsOK = true;
  if( mQFactor < numeric_limits<Real>::epsilon() )
  {
    bcierr << "Q factor must be greater 0" << endl;
    paramsOK = false;
  }
  if( mCenterFreq <= 0 || mCenterFreq >= 0.5 )
  {
    bcierr << "Center frequency must be > 0 and < 0.5" << endl;
    paramsOK = false;
  }
  ListOfComplex zeros, poles;
  if( paramsOK )
  {
    const int cMaxIterations = 50;
    const Real cConvergenceEps = 1e-10;
    zeros.resize( 2 );
    zeros[ 0 ] = 1;
    zeros[ 1 ] = -1;
    Real theta = 2.0 * M_PI * mCenterFreq; /* where we want the peak to be */
    /* iterate to find exact pole positions */
    Real r = ::exp( -theta / 2.0 / mQFactor ),
         thm = theta,
         th1 = 0.0,
         th2 = M_PI;
    poles.resize( 2 );
    bool converged = false;
    for( int i = 0; i < cMaxIterations && !converged; ++i )
    {
      poles[ 0 ] = std::polar( r, thm );
      poles[ 1 ] = std::polar( r, -thm );
      Ratpoly<Complex> tf( Polynomial<Complex>::FromRoots( zeros ),
                           Polynomial<Complex>::FromRoots( poles ) );
      Complex g = tf.Evaluate( std::polar( 1, theta ) );
      Real phi = g.imag() / g.real();
      if( phi > 0.0 )
        th2 = thm;
      else
        th1 = thm;
      converged = ( ::fabs( phi ) < cConvergenceEps );
      thm = ( th1 + th2 ) / 2.0;
    }
    if( !converged )
      bcierr << "Failed to converge" << endl;

    switch( mCharacter )
    {
      case bandpass:
        break;

      case bandstop:
        zeros[ 0 ] = std::polar( 1, theta );
        zeros[ 1 ] = std::polar( 1, -theta );
        break;

      case allpass:
        zeros[ 0 ] = poles[ 0 ] / sqr( abs( poles[ 0 ] ) );
        zeros[ 1 ] = poles[ 1 ] / sqr( abs( poles[ 1 ] ) );
        break;

      default:
        bcierr << "Unsupported filter character" << endl;
    }
  }
  return Ratpoly<Complex>(
           Polynomial<Complex>::FromRoots( zeros ),
           Polynomial<Complex>::FromRoots( poles, -1 )
         );
}

}; // namespace FilterDesign