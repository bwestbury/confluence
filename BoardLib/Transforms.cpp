/* -*- mode: c++ -*- */
/**
 * @file   Transforms.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  
 * @copyright
 */

#include "Rect.h"
#include "Shapes.h"
#include "Transforms.h"
#include <cmath>

double round(double x)//[Added by Sebastian Jörn]
{
if(x>=0.5){return ceil(x);}else{return floor(x);}
}

namespace BoardLib {

//
// Transform
// 

float
Transform::rounded( float x ) const
{
  return round( 10000*x ) / 10000;
} 

float
Transform::mapX( float x ) const
{
  return rounded( x * _scale + _deltaX );
}

float
Transform::scale( float x ) const
{
  return rounded( x * _scale );
}

void
Transform::apply( float & x, float & y ) const
{
  x = mapX( x );
  y = mapY( y );
}

void
Transform::furtherScale( float x )
{
  _scale *= x;
}

//
// TransformEPS
// 

float
TransformEPS::mapY( float y ) const
{
  return rounded( y * _scale + _deltaY );
}

void
TransformEPS::setBoundingBox( const Rect & rect )
{
  float ppmm = (720/254.0f);  
  if ( rect.height/rect.width > 27.7/19.0 ) {
    _scale = 277 * ppmm / rect.height;
  } else {
    _scale = 190 * ppmm / rect.width;
  }
  _deltaX = 0.5 * 210 * ppmm - _scale * ( rect.left + 0.5 * rect.width );
  _deltaY = 0.5 * 297 * ppmm - _scale * ( rect.top - 0.5 * rect.height );  
}

//
// TransformFIG
// 

float
TransformFIG::rounded( float x ) const
{
  return round( x );
}

float
TransformFIG::mapY( float y ) const
{
  // float ppmm = 1200/25.4;
  float ppmm = 1143/25.4f;
  return rounded( ( 297*ppmm ) - ( y * _scale + _deltaY ) );
}

int
TransformFIG::mapWidth( float width ) const
{
  // FIG width unit is 1/160 inch
  // Postscript points are 1/72 inch
  if ( width == 0.0 ) return 0;
  int result = static_cast<int>( round( 160 * (width / 72.0 ) ) );
  return result>0?result:1;
}

void
TransformFIG::setBoundingBox( const Rect & rect )
{
  // float ppmm = (1200/25.4);
  float ppmm = (1143/25.4f);
  if ( rect.height/rect.width > 27.7/19.0 ) {
    _scale = 277 * ppmm / rect.height;
  } else {
    _scale = ( 190 * ppmm ) / rect.width;
  }
  _deltaX = 0.5 * 210 * ppmm - _scale * ( rect.left + 0.5 * rect.width );
  _deltaY = 0.5 * 297 * ppmm - _scale * ( rect.top - 0.5 * rect.height );  
}

void
TransformFIG::setDepthRange( const std::vector<Shape*> & shapes )
{
  maxDepth = 0;
  minDepth = std::numeric_limits<int>::max();
  std::vector<Shape*>::const_iterator i = shapes.begin();
  std::vector<Shape*>::const_iterator end = shapes.end();
  while ( i != end ) {
    if ( (*i)->depth() > maxDepth ) maxDepth = (*i)->depth();
    if ( (*i)->depth() < minDepth ) minDepth = (*i)->depth();
    ++i;
  }
}

int
TransformFIG::mapDepth( int depth ) const
{
  if ( depth > maxDepth ) return 999;
  if ( maxDepth - minDepth > 998 ) {
    double range = maxDepth - minDepth;
    return static_cast<int>( 1 + round( ( ( depth - minDepth ) / range ) * 998 ) );
  } else {
    return 1 + depth - minDepth;
  }
}

//
// TransformSVG
// 

float
TransformSVG::rounded( float x ) const
{
  return round( 100*x ) / 100.0f;
} 

float
TransformSVG::mapY( float y ) const
{
  const float ppmm = 720/254.0f;
  return rounded( ( 297 * ppmm ) - ( y * _scale + _deltaY ) );
}

float
TransformSVG::mapWidth( float width ) const
{
  const float ppmm = 72.0/25.4f;
  return round( 1000 * width / ppmm ) / 1000.0;
}

void
TransformSVG::setBoundingBox( const Rect & rect )
{
  const float ppmm = 720/254.0f;
  if ( rect.height/rect.width > 27.7/19.0 ) {
    _scale = 277 * ppmm / rect.height;
  } else {
    _scale = 190 * ppmm / rect.width;
  }
  _deltaX = 0.5 * 210 * ppmm - _scale * ( rect.left + 0.5 * rect.width );
  _deltaY = 0.5 * 297 * ppmm - _scale * ( rect.top - 0.5 * rect.height );
}

} // namespace BoardLib
