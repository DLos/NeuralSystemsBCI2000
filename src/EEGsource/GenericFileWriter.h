////////////////////////////////////////////////////////////////////////////////
//
// File: GenericFileWriter.h
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Date: Jun 17, 2005
//
// Description: A virtual class interface for output filters encapsulating
//       output file formats.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GenericFileWriterH
#define GenericFileWriterH

#include "UGenericFilter.h"

class GenericFileWriter : public GenericFilter
{
 protected:
  GenericFileWriter() {}

 public:
  virtual ~GenericFileWriter() {}
  // GenericFilter inherited functions.
  virtual void Preflight( const SignalProperties&,
                                SignalProperties& ) const = 0;
  virtual void Initialize() = 0;
  virtual void StartRun() {}
  virtual void StopRun() {}
  // The write function takes as argument the state vector
  // that existed at the time of the signal argument's time stamp.
  virtual void Write(   const GenericSignal&, const STATEVECTOR& ) = 0;
  virtual void Halt() {}

 private:
  // The Process() function should not be called for the
  // GenericFileWriter class.
  virtual void Process(   const GenericSignal*,
                                GenericSignal* ) {};
};

#endif // GenericFileWriterH

