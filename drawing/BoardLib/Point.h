/* -*- mode: c++ -*- */
/**
 * @file   Point.h
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  The Point structure.
 * @copyright
 * Copyright or © or Copr. Sébastien Fourey
 * 
 * Sébastien.Fourey - http://www.greyc.ensicaen.fr/~seb
 * 
 * 
 * This source code is part of the Board project, a C++ library whose
 * purpose is to allow simple drawings in EPS, FIG or SVG files.
 * 
 * This software is governed by the CeCILL-C license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL-C
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C license and that you accept its terms
 * 
 */
#ifndef _BOARD_POINT_H_
#define _BOARD_POINT_H_

#include <cmath>

namespace BoardLib {

/**
 * The Point structure.
 * @brief Struct representing a 2D point. 
 */
struct Point {
  double x;			/**< The point's first coordinate */
  double y;			/**< The point's second coordinate */
  /** 
   * Point constructor.
   * 
   * @param x The point's first coordinate.
   * @param y The point's second coordinate.
   */
  Point( double x = 0.0, double y = 0.0 ):x(x),y(y) { } 

  /** 
   * Rotate the point with a given angle around the origin (0,0)
   * 
   * @param angle The rotation angle.
   */
  inline Point & rotate( double angle );

  /** 
   * Get the values of the x and y values.
   * 
   * @param x Reference to the x to be set.
   * @param y Reference to the y to be set.
   */
  inline void get( double & x, double & y ) const;
  
  /** 
   * Rotate the point with a given angle around the origin (0,0)
   * 
   * @param angle The rotation angle.
   * 
   * @return The rotated point.
   */
  inline Point getRotate( double angle ) const;

  inline Point & rotate( double angle, const Point & center );

  inline Point & getRotate( double angle, const Point & center ) const;

  inline Point & operator+=( const Point & other );

  inline Point & operator-=( const Point & other );

  inline Point & operator*=( double s );

  inline Point & operator/=( double s );

  inline double norm() const;
  
};

inline void
Point::get( double & x, double & y ) const
{
  x = Point::x;
  y = Point::y;
}

inline Point
operator+( const Point & a, const Point & b )
{
  return Point( a.x + b.x, a.y + b.y );
}

inline Point
operator-( const Point & a, const Point & b )
{
  return Point( a.x - b.x, a.y - b.y );
}

inline double
operator*( const Point & a, const Point & b )
{
  return a.x * b.x + a.y * b.y;
}

inline Point
operator*( const Point & p, double s  )
{
  return Point( p.x * s, p.y * s );
}

inline Point
operator*( double s, const Point & p )
{
  return Point( s * p.x, s * p.y );
}

inline Point
operator/( const Point & p, double s )
{
  return Point( p.x / s, p.y / s );
}

inline Point &
Point::operator+=( const Point & other )
{
  x += other.x;
  y += other.y;
  return *this;
}

inline Point &
Point::operator-=( const Point & other )
{
  x -= other.x;
  y -= other.y;
  return *this;
}

inline Point &
Point::operator*=( double s )
{
  x *= s;
  y *= s;
  return *this;
}

inline Point &
Point::operator/=( double s )
{
  x /= s;
  y /= s;
  return *this;
}

inline bool
operator==( const Point & a, const Point & b )
{
  return ( a.x == b.x ) && ( a.y == b.y ) ;
}

inline bool
operator!=( const Point & a, const Point & b )
{
  return ( a.x != b.x ) || ( a.y != b.y ) ;
}

Point &
Point::rotate( double angle )
{
  double x = cos( angle ) * Point::x - sin( angle ) * Point::y; 
  double y = sin( angle ) * Point::x + cos( angle ) * Point::y;
  Point::x = x;
  Point::y = y;
  return *this;
}

Point
Point::getRotate( double angle ) const
{
  return Point(*this).rotate( angle );
}

Point &
Point::rotate( double angle, const Point & center )
{
  (*this) -= center;
  (*this).rotate( angle );
  (*this) += center;
  return *this;
}

Point &
Point::getRotate( double angle, const Point & center ) const
{
  return Point(*this).rotate( angle, center );
}

double
Point::norm() const
{
  return sqrt( x*x + y*y );
}

} // mamespace BoardLib

#endif // _POINT_H_

