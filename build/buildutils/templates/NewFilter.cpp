////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: ` implementation
//   
//   
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2011: BCI2000 team and many external contributors ]
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "`.h"
#include "BCIError.h"


using namespace std;


RegisterFilter( `, 2.M );
     // Change the location as appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filters locations for SignalSource modules begin with "1."
     //  - filters locations for SignalProcessing modules begin with "2."  
     //       (but the order is often explicitly re-defined in a PipeDefinition.cpp file)
     //  - filters locations Application modules begin with "3."


`::`()
{
  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

   // "Filtering int Enable`= 0 0 0 1 // enable `? (boolean)",

 END_PARAMETER_DEFINITIONS

  // ...and any state variables:
  
 BEGIN_STATE_DEFINITIONS
 
   // "SomeState 0 0 0 0",
   
 END_STATE_DEFINITIONS

}


`::~`()
{
  Halt();
}

void
`::Halt()
{
  // De-allocate any memory reserved in Initialize, stop any threads, etc.
  // Good practice is to write this such that it is safe to call even *before* Initialize,
  // or safe to call twice (e.g. make sure you do not delete [] pointers that have already been
  // deleted:  set them to NULL after deletion).
}

void
`::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  //
  // Parameter( "Milk" );
  // State( "Bananas" );
  //
  // Also check that the values of any parameters are sane:
  //
  // if( (float)Parameter( "Karma" ) == 0.0f )
  //      bcierr << "zero Karma is not allowed" << endl;
  // 
  // Errors issued in this way, during Preflight, still allow the user to open
  // the Config dialog box, fix bad parameters and re-try.  By contrast, errors
  // and C++ exceptions at any other stage will make the system stop, such that
  // BCI2000 will need to be relaunched entirely.
  
  Output = Input; // this simply passes information about through SampleBlock dimensions, etc....
  
  // ... or alternatively, we could modify that info here:

  // Let's say this filter has only one output, namely the amount of stuff detected in the signal.
  // Output.SetChannels( 1 );
  // Output.ChannelLabels()[0] = "Stuff";

  // Just for fun, let's output twice as many samples (or bins) as we receive from the input.
  // Output.SetElements( Input.Elements() * 2 );
  
  // Note that the ` instance itself, and its members, are read-only during
  // this phase, due to the "const" at the end of the Preflight prototype above.
  // Any methods called by Preflight must also be "const" in the same way.
}


void
`::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the ` instance itself can be modified. Allocate any memory you need, start any
  // threads, store any information you need in private member variables.
}

void
`:StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  bciout << "Hello World!" << endl;
}


void
`::Process( const GenericSignal& Input, GenericSignal& Output )
{

  // And now we're processing a single SampleBlock of data.
  
  Output = Input; // Pass the signal through unmodified.
                  // ( Obviously this will no longer fly if we modified the shape of the
                  //   output SignalProperties during Preflight ).
                  
  // Or we could do it one value at a time:
  /*
  for( ch = 0; ch < Output.Channels(); ch++ )
  {
    for( el = 0; el < Output.Elements(); el++ )
    {
      Output( ch, el ) = some_function( Input );
    }
  }
  */
}

void
`:StopRun()
{
  bciout << "Goodbye World." << endl;
  // You know, you can delete methods if you're not using them.
  // Remove the declaration from `.h too, if so.
}
