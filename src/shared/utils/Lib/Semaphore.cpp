//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for semaphore objects.
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
///////////////////////////////////////////////////////////////////////
#include "Semaphore.h"

#if _WIN32
# include <windows.h>
#else
# include <cerrno>
#endif // _WIN32

#if _WIN32
Semaphore::Semaphore( int inCount )
: mHandle( ::CreateSemaphore( NULL, inCount, inCount, NULL ) )
{
}

Semaphore::~Semaphore()
{
  if( mHandle != NULL )
    ::CloseHandle( mHandle );
}

bool
Semaphore::Acquire() const
{
  return ( WAIT_OBJECT_0 == ::WaitForSingleObject( mHandle, INFINITE ) );
}

bool
Semaphore::Release() const
{
  return ::ReleaseSemaphore( mHandle, 1, 0 );
}

#else // _WIN32

Semaphore::Semaphore( int inCount )
{
  ::sem_init( &mSemaphore, 0, inCount );
}

Semaphore::~Semaphore()
{
  ::sem_destroy( &mSemaphore );
}

bool
Semaphore::Acquire() const
{
  int err = EINTR;
  while( err == EINTR )
    err = ::sem_wait( const_cast<sem_t*>( &mSemaphore ) ) ? errno : 0;
  return 0 == err;
}

bool
Semaphore::Release() const
{
  return 0 == ::sem_post( const_cast<sem_t*>( &mSemaphore ) );
}

#endif // _WIN32
