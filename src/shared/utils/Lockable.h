//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes/templates related to locking.
//   *Lockable is a mixin-class to add locking to a class,
//   *Lock<T> defines RAAI objects that lock a lockable
//    object during the lifetime of the RAAI object.
//    For a RAAI class that operates on an OSMutex rather than
//    a Lockable, see OSMutex::Lock.
//   *TemporaryLock<T>() is a function template that returns a
//    temporary Lock<T> object. This allows, e.g., to lock a stream
//    object during evaluation of an expression that writes into the
//    stream object.
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
#ifndef LOCKABLE_H
#define LOCKABLE_H

#include "OSMutex.h"
#include "BCIAssert.h"
#include "Uncopyable.h"

class Lockable
{
 public:
  Lockable() {}
  Lockable( const Lockable& ) {}
  virtual ~Lockable() {}
  Lockable& operator=( const Lockable& ) { return *this; }

  bool Lock() const   { return mMutex.Acquire(); }
  bool Unlock() const { return mMutex.Release(); }

 private:
  OSMutex mMutex;
};

template<typename T> class Lock_;

template<typename T>
class Lock_
{
 private:
  Lock_();
  Lock_& operator=( const Lock_& );

 public:
  Lock_( T& t )
    : mpT( &t ), mcpT( 0 )
    { t.Lock(); }
  Lock_( const T& t )
    : mpT( 0 ), mcpT( &t )
    { t.Lock(); }
  Lock_( T* pT )
    : mpT( pT ), mcpT( 0 )
    { pT->Lock(); }
  Lock_( const T* cpT )
    : mpT( 0 ), mcpT( cpT )
    { cpT->Lock(); }
  // A copy constructor is required by some compilers that need it to implement
  //   return Lock_( x );
  // An object of type T must allow recursive locking, otherwise the copy
  // constructor will block.
  Lock_( const Lock_& other )
    : mpT( other.mpT ), mcpT( other.mcpT )
    { if( mpT ) mpT->Lock(); else if( mcpT ) mcpT->Lock(); }
  ~Lock_()
    { if( mpT ) mpT->Unlock(); else if( mcpT ) mcpT->Unlock(); }

  const T& ConstRef() const
    { return mpT ? *mpT : *mcpT; }
  T& MutableRef() const
    { bciassert( mpT ); return *mpT; }
  T& operator()() const
    { return MutableRef(); }

 private:
  T* mpT;
  const T* mcpT;
};

template<typename T>
class Lock_<const T> : public Lock_<T>
{
  public:
   Lock_( const T* t ) : Lock_<T>( t ) {}
   Lock_( const T& t ) : Lock_<T>( t ) {}
   const T& operator()() const
    { return Lock_<T>::ConstRef(); }
};

template<typename T>
Lock_<T>
TemporaryLock( T& t )
{ // Some compilers require a copy constructor here.
  return Lock_<T>( t );
}

class Lock : Uncopyable
{
 public:
  Lock( const Lockable& lockable ) : mpLockable( &lockable ), mpProxy( 0 ) { mpLockable->Lock(); }
  Lock( const Lockable* pLockable ) : mpLockable( pLockable ), mpProxy( 0 ) { mpLockable->Lock(); }
  template<class T> Lock( T& t ) : mpLockable( 0 ), mpProxy( new LockProxy_<T>( t ) ) {}
  template<class T> Lock( T* t ) : mpLockable( 0 ), mpProxy( new LockProxy_<T>( *t ) ) {}
  ~Lock() { if( mpLockable ) mpLockable->Unlock(); else delete mpProxy; }
 private:
  struct LockProxy { virtual ~LockProxy() {} };
  template<class T> struct LockProxy_ : LockProxy
  {
    LockProxy_( T& t ) : t( t ) { t.Lock(); }
    ~LockProxy_() { t.Unlock(); }
    T& t;
  };
  const LockProxy* mpProxy;
  const Lockable* mpLockable;
};

#endif // LOCKABLE_H

