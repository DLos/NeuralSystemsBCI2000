////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: geomObj holds common geometric properties of 3D scene objects,
//   and provides an interface for hit testing (volume intersection).
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GEOM_OBJ_H
#define GEOM_OBJ_H

#include "component.h"

class geomObj
{
 private:
  GLfloat  mWorldTransform[16];  // Matrix representing the object's position,
                                 // orientation, and scale in space.
  CVector3 mVelocity,            // Velocities of movement
           mAngVelocity;

 public:
  geomObj();
  virtual ~geomObj() {}
  const GLfloat* getWorldTransform() const
        { return mWorldTransform; }

  // Set/get the current proper system's origin and axes as stored in the
  // world transform matrix.
  //  Typically, the proper system's origin is the object's center of mass.
  //  For convex objects, it is assumed that the origin belongs to it
  //  (in the sense that it is part of the object's solid volume).
  void setOrigin( const CVector3& );
  void setOrigin( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setOrigin( c ); }
  CVector3 getOrigin() const;
  float getOriginX() const
        { return getOrigin().x; }
  float getOriginY() const
        { return getOrigin().y; }
  float getOriginZ() const
        { return getOrigin().z; }

  void setXAxis( const CVector3& );
  void setXAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setXAxis( c ); }
  CVector3 getXAxis() const;
  float getXAxisX() const
        { return getXAxis().x; }
  float getXAxisY() const
        { return getXAxis().y; }
  float getXAxisZ() const
        { return getXAxis().z; }

  void setYAxis( const CVector3& );
  void setYAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setYAxis( c ); }
  CVector3 getYAxis() const;
  float getYAxisX() const
        { return getYAxis().x; }
  float getYAxisY() const
        { return getYAxis().y; }
  float getYAxisZ() const
        { return getYAxis().z; }

  void setZAxis( const CVector3& );
  void setZAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setZAxis( c ); }
  CVector3 getZAxis() const;
  float getZAxisX() const
        { return getZAxis().x; }
  float getZAxisY() const
        { return getZAxis().y; }
  float getZAxisZ() const
        { return getZAxis().z; }

  // Movement parameters
  void setVelocity( const CVector3& v )
        { mVelocity = v; }
  void setVelocity( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setVelocity( c ); }
  const CVector3& getVelocity() const
        { return mVelocity; }
  void setAngVelocity( const CVector3& v )
        { mAngVelocity = v; }
  void setAngVelocity( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setAngVelocity( c ); }
  const CVector3& getAngVelocity() const
        { return mAngVelocity; }

  // Transformations of the object's coordinate system
  // Translate
  void translate( const CVector3& );
  void translate( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; translate( c ); }
  // Rotate around an axis through the global origin
  void rotate( const CVector3& vecAngle );
  void rotate( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; rotate( c ); }
  void rotate( GLfloat angle, const CVector3& axis );
  void rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; rotate( angle, c ); }
  // Rotate around an axis through the object's center
  void spin( const CVector3& vecAngle );
  void spin( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; spin( c ); }
  void spin( GLfloat angle, const CVector3& axis );
  void spin( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; spin( angle, c ); }
  // Scale without affecting the object's position
  void scale( const CVector3& v )
        { scale( v.x, v.y, v.z ); }
  void scale( GLfloat x, GLfloat y, GLfloat z );

  // Geometric properties related to hit testing
  enum testResult { false_ = 0, true_ = 1, undetermined };
  virtual testResult isConvex() const
        { return undetermined; };
  virtual testResult contains( const CVector3& ) const
        { return undetermined; };
  virtual testResult intersectsVolume( const geomObj& ) const
        { return undetermined; };
  static bool VolumeIntersection( const geomObj&, const geomObj& );

  // Events for geomObjs:
  void move( float dt )
        { onMove( dt ); }

 protected:
  virtual void onMove( float dt )
        { translate( dt*mVelocity ); spin( dt*mAngVelocity ); }
};//geomObj

#endif // GEOM_OBJ_H


