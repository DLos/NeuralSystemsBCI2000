////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: An abstract base class that implements an IIR filter.
//   Subclasses will provide individual implementations for the
//   DesignFilter() member, which is supposed to translate parameter settings
//   into a rational transfer function (complex poles and zeros).
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef IIR_FILTER_BASE_H
#define IIR_FILTER_BASE_H

#include "GenericFilter.h"
#include "IIRFilter.h"
#include "ReusableThread.h"

class IIRFilterBase : public GenericFilter
{
 public:
  typedef double                         Real;
  typedef IIRFilter<Real>::ComplexVector ComplexVector;

 protected:
  IIRFilterBase();
  ~IIRFilterBase();

 public:
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void StartRun();
  void Process( const GenericSignal&, GenericSignal& );

 private:
  // Translate user settings into a filter definition given by
  // - overall gain,
  // - complex roots of the numerator polynomial ("zeros"),
  // - complex roots of the denominator polynomial ("poles").
  virtual void DesignFilter( SignalProperties& signal,
                             Real& gain,
                             ComplexVector& zeros,
                             ComplexVector& poles ) const = 0;

  struct ChannelSet
  {
    int Channels() const
      { return end - begin; }
    // Input
    GenericSignal::ValueType operator()( int ch, int el ) const
      { return (*pInput)( begin + ch, el ); }
    int Elements() const
      { return pInput->Elements(); }
    // Output
    GenericSignal::ValueType& operator()( int ch, int el )
      { return (*pOutput)( begin + ch, el ); }
    int Elements()
      { return pOutput->Elements(); }
    // Assignment
    void operator=( const ChannelSet& );

    const GenericSignal* pInput;
    GenericSignal* pOutput;
    int begin, end;
  };
  struct Filter : Runnable, IIRFilter<Real>
  {
    ChannelSet channels;
    void OnRun() { IIRFilter<Real>::Process( channels, channels ); }
  } mFilter;
  struct FilterThread : ReusableThread
  {
    Filter filter;
  } *mpThreads;
  int mAdditionalThreads;

};

#endif // IIR_FILTER_BASE_H
