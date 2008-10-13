/* -*- mode: c++ -*- */
/**
 * @file   Shapes.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  
 * @copyright
 */
#include "Rect.h"
#include "Shapes.h"
#include <cmath>
#include <vector>
#include <sstream>

namespace BoardLib {

extern const char * XFigPostscriptFontnames[];

bool
shapeGreaterDepth( const Shape *s1, const Shape *s2 )
{
  return s1->depth() > s2->depth();
}

std::string
Shape::svgProperties( const TransformSVG & transform ) const
{
  static const char * capStrings[3] = { "butt", "round", "square" };
  static const char * joinStrings[3] = { "miter", "round", "bevel" };
  std::stringstream str;
  if ( _penColor != Color::none ) {
    str << " fill=\"" << _fillColor.svg() << '"'
	<< " stroke=\"" << _penColor.svg() << '"'
	<< " stroke-width=\"" << transform.mapWidth( _lineWidth ) << "mm\""
	<< " style=\"stroke-linecap:" << capStrings[ _lineCap ]  
	<< ";stroke-linejoin:" << joinStrings[ _lineJoin ] << '"'
	<< _fillColor.svgAlpha( " fill" )
	<< _penColor.svgAlpha( " stroke" );
  } else  {
    str << " fill=\"" << _fillColor.svg() << '"'
	<< " stroke=\"" << _fillColor.svg() << '"'
	<< " stroke-width=\"0.5px\""
	<< " style=\"stroke-linecap:round;stroke-linejoin:round;\""
	<< _fillColor.svgAlpha( " fill" )
	<< _fillColor.svgAlpha( " stroke" );
  }
  return str.str();
}
 
std::string
Shape::postscriptProperties() const
{
  std::stringstream str;
  str << _lineWidth << " slw ";
  str << _lineCap << " slc ";
  str << _lineJoin << " slj";
  return str.str();
}

void
Shape::depth( int d )
{
  _depth = d;
}

Point
Line::center() const {
  return 0.5 * Point( _x1 + _x2, _y1 + _y2 );
}

Shape &
Line::rotate( double angle, const Point & center )
{
  Point( _x1, _y1 ).rotate( angle, center ).get( _x1, _y1 );
  Point( _x2, _y2 ).rotate( angle, center ).get( _x2, _y2 );
  return *this;
}

Shape &
Line::rotate( double angle )
{
  return rotate( angle, center() );
}

Shape &
Line::translate( double dx, double dy )
{
  _x1 += dx; _x2 += dx;
  _y1 += dy; _y2 += dy;
  return *this;
}

Shape &
Line::scale( double sx, double sy )
{
  Point c = center();
  _x1 *= sx;
  _x2 *= sx;
  _y1 *= sy;
  _y2 *= sy;
  Point delta = c - center();
  translate( delta.x, delta.y );
  return *this;
}

void
Line::flushPostscript( std::ostream & stream,
		       const TransformEPS & transform ) const
{
  stream << postscriptProperties() << " "
	 << "n "
	 << transform.mapX( _x1 ) << " " 
	 << transform.mapY( _y1 ) << " " 
	 << "m "
	 << transform.mapX( _x2 ) << " " 
	 << transform.mapY( _y2 ) << " " 
	 << "l " << _penColor.postscript() << " srgb stroke" << std::endl;
}

void
Line::flushFIG( std::ostream & stream,
		const TransformFIG & transform,
		std::map<Color,int> & colormap ) const
{
  stream << "2 1 0 ";
  // Thickness
  stream << ( _penColor.valid()?transform.mapWidth( _lineWidth ):0 ) << " ";
  // Pen color
  stream << colormap[ _penColor ] << " ";
  // Fill color
  stream << "0 ";
  // Depth
  stream << transform.mapDepth( _depth ) << " ";
  // Pen style
  stream <<  "-1 ";
  // Area fill, style val, join style, cap style, radius, f_arrow, b_arrow
  stream << "-1 0.000 " << _lineJoin << " " << _lineCap << " -1 0 0 ";
  // Number of points
  stream << "2\n";
  stream << "         ";
  stream << static_cast<int>( transform.mapX( _x1 ) ) << " " 
	 << static_cast<int>( transform.mapY( _y1 ) ) << " "
	 << static_cast<int>( transform.mapX( _x2 ) ) << " " 
	 << static_cast<int>( transform.mapY( _y2 ) ) << std::endl;
}

void
Line::flushSVG( std::ostream & stream,
		const TransformSVG & transform ) const
{
  stream << "<line x1=\"" << transform.mapX( _x1 ) << "\""
	 << " y1=\"" << transform.mapY( _y1 ) << "\""
	 << " x2=\"" << transform.mapX( _x2 ) << "\""
	 << " y2=\"" << transform.mapY( _y2 ) << "\""
    	 << svgProperties( transform ) 
	 << " />" << std::endl;
}

Rect
Line::boundingBox() const
{
  Rect rect;
  if ( _x1 > _x2 ) {
    rect.width = _x1 - _x2;
    rect.left = _x2;
  } else {
    rect.width = _x2 - _x1;
    rect.left = _x1;
  }
  if ( _y1 > _y2 ) {
    rect.top = _y1;
    rect.height = _y1 - _y2;
  } else {
    rect.top = _y2;
    rect.height = _y2 - _y1;
  }
  return rect;
}

void
Arrow::flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const
{
  double dx = _x1 - _x2;
  double dy = _y1 - _y2;
  double norm = sqrt( dx*dx + dy*dy );
  dx /= norm;
  dy /= norm;
  dx *= 10*_lineWidth;
  dy *= 10*_lineWidth;
  double ndx1 = dx*cos(0.3)-dy*sin(0.3);
  double ndy1 = dx*sin(0.3)+dy*cos(0.3);
  double ndx2 = dx*cos(-0.3)-dy*sin(-0.3);
  double ndy2 = dx*sin(-0.3)+dy*cos(-0.3);
    
  stream << _penColor.postscript() << " srgb "
	 << postscriptProperties() << " "
	 << "n "
	 << transform.mapX( _x1 ) << " " 
	 << transform.mapY( _y1 ) << " " 
	 << "m "
	 << transform.mapX( _x2 ) << " " 
	 << transform.mapY( _y2 ) << " " 
	 << "l stroke" << std::endl;

  if ( filled() ) { 
    stream << "n "
	   << transform.mapX( _x2 ) + ndx1 << " " 
	   << transform.mapY( _y2 ) + ndy1 << " " 
	   << "m "
	   << transform.mapX( _x2 ) << " " 
	   << transform.mapY( _y2 ) << " l " 
	   << transform.mapX( _x2 ) + ndx2 << " " 
	   << transform.mapY( _y2 ) + ndy2 << " ";
    stream  << "l cp " << _penColor.postscript() << " srgb  fill" << std::endl;
  }
  
  stream << "n "
	 << transform.mapX( _x2 ) + ndx1 << " " 
	 << transform.mapY( _y2 ) + ndy1 << " " 
	 << "m "
	 << transform.mapX( _x2 ) << " " 
	 << transform.mapY( _y2 ) << " l " 
	 << transform.mapX( _x2 ) + ndx2 << " " 
	 << transform.mapY( _y2 ) + ndy2 << " l"
	 << " " << _penColor.postscript() << " srgb stroke" << std::endl;
}

void
Arrow::flushFIG( std::ostream & stream,
		const TransformFIG & transform,
		std::map<Color,int> & colormap ) const
{
  stream << "2 1 0 ";
  // Thickness
  stream << ( _penColor.valid()?transform.mapWidth( _lineWidth ):0 ) << " ";
  // Pen color
  stream << colormap[ _penColor ] << " ";
  // Fill color
  stream << colormap[ _penColor ] << " ";
  // Depth
  stream << transform.mapDepth( _depth ) << " ";
  // Pen style
  stream <<  "-1 ";
  // Area fill, style val, join style, cap style, radius, f_arrow, b_arrow
  stream << "-1 0.000 " << _lineJoin << " " << _lineCap << " -1 1 0 ";
  // Number of points
  stream << "2\n";
  if ( filled() ) 
    stream << "       1 1 1.00 60.00 120.00\n";
  else 
    stream << "       0 0 1.00 60.00 120.00\n";
  stream << "         ";
  stream << static_cast<int>( transform.mapX( _x1 ) ) << " " 
	 << static_cast<int>( transform.mapY( _y1 ) ) << " "
	 << static_cast<int>( transform.mapX( _x2 ) ) << " " 
	 << static_cast<int>( transform.mapY( _y2 ) ) << std::endl;
}

void
Arrow::flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const
{
  double dx = _x1 - _x2;
  double dy = _y1 - _y2;
  double norm = sqrt( dx*dx + dy*dy );
  dx /= norm;
  dy /= norm;
  dx *= 10*_lineWidth;
  dy *= 10*_lineWidth;
  double ndx1 = dx*cos(0.3)-dy*sin(0.3);
  double ndy1 = dx*sin(0.3)+dy*cos(0.3);
  double ndx2 = dx*cos(-0.3)-dy*sin(-0.3);
  double ndy2 = dx*sin(-0.3)+dy*cos(-0.3);
  
  stream << "<g>" << std::endl;
  // The line
  stream << " <path "
	 << "d=\"M " << transform.mapX( _x1 ) << " " << transform.mapY( _y1 )
	 << " L " << transform.mapX( _x2 ) << " " << transform.mapY( _y2 ) << " z\""
	 << " fill=\"none\" stroke=\"" << _penColor.svg() << "\""
    	 << _penColor.svgAlpha( " stroke" )
	 << " stroke-width=\"" << transform.mapWidth( _lineWidth ) << "mm\" />";
  
  // The arrow
  stream << " <polygon";
  stream << " fill=\"" << _fillColor.svg() << "\"";
  stream << " stroke=\"" << _penColor.svg() << "\""
	 << " stroke-width=\"" << transform.mapWidth( 0.33 * _lineWidth ) << "mm\""
	 << " style=\"stroke-linecap:butt;stroke-linejoin:miter\""
	 << _fillColor.svgAlpha( " fill" )
	 << _penColor.svgAlpha( " stroke" )
	 << " points=\""
	 << transform.mapX( _x2 ) + ndx1 << "," 
	 << transform.mapY( _y2 ) - ndy1 << " "
	 << transform.mapX( _x2 ) << "," 
	 << transform.mapY( _y2 ) << " " 
	 << transform.mapX( _x2 ) + ndx2 << "," 
	 << transform.mapY( _y2 ) - ndy2 << " "
	 << transform.mapX( _x2 ) + ndx1 << "," 
	 << transform.mapY( _y2 ) - ndy1 << "\" />" << std::endl;
  stream << "</g>" << std::endl;
}

void
Rectangle::flushFIG( std::ostream & stream,
		     const TransformFIG & transform,
		     std::map<Color,int> & colormap ) const
{
  if ( _points[0].y != _points[1].y ) {
    Polyline::flushFIG( stream, transform, colormap );
    return;
  }
  stream << "2 2 0 ";
  // Thickness
  stream << ( _penColor.valid()?transform.mapWidth( _lineWidth ):0 ) << " ";
  // Pen color
  stream << colormap[ _penColor ] << " ";
  // Fill color
  stream << colormap[ _fillColor ] << " ";
  // Depth
  stream << transform.mapDepth( _depth ) << " ";
  // Pen style
  stream <<  "-1 ";
  // Area fill, style val, join style, cap style, radius, f_arrow, b_arrow, number of points
  if ( filled() ) 
    stream << "20 0.000 " << _lineJoin << " " << _lineCap << " -1 0 0 5\n";
  else
    stream << "-1 0.000 " << _lineJoin << " " << _lineCap << " -1 0 0 5\n";
  stream << "         ";
  
  stream << static_cast<int>( transform.mapX( _points[0].x ) ) << " "
	 << static_cast<int>( transform.mapY( _points[0].y ) ) << " "
	 << static_cast<int>( transform.mapX( _points[1].x ) ) << " "
	 << static_cast<int>( transform.mapY( _points[1].y ) ) << " "
	 << static_cast<int>( transform.mapX( _points[2].x ) ) << " "
	 << static_cast<int>( transform.mapY( _points[2].y ) ) << " "
	 << static_cast<int>( transform.mapX( _points[3].x ) ) << " "
	 << static_cast<int>( transform.mapY( _points[3].y ) ) << " "
	 << static_cast<int>( transform.mapX( _points[0].x ) ) << " "
	 << static_cast<int>( transform.mapY( _points[0].y ) ) << "\n";
}

void
Rectangle::flushSVG( std::ostream & stream,
		     const TransformSVG & transform ) const
{
  if ( _points[0].y == _points[1].y ) {
    stream << "<rect x=\"" << transform.mapX( _points[0].x ) << '"'
	   << " y=\"" << transform.mapY( _points[0].y )  << '"'
	   << " width=\"" << transform.scale( _points[1].x - _points[0].x ) << '"'
	   << " height=\"" << transform.scale( _points[0].y - _points[3].y ) << '"'
	   << svgProperties( transform ) 
	   << " />" << std::endl;
  } else {
    Point v = _points[1] - _points[0];
    v /= v.norm();
    double angle = ( _points[1].y > _points[0].y ) ? acos( v * Point(1,0) ) : -acos( v * Point( 1, 0 ) );
    angle = ( angle * 180 ) / M_PI;
    stream << "<rect x=\"" << transform.mapX( _points[0].x ) << '"'
	   << " y=\"" << transform.mapY( _points[0].y )  << '"'
	   << " width=\"" << transform.scale( (_points[1] - _points[0]).norm() ) << '"'
	   << " height=\"" << transform.scale( (_points[0] - _points[3]).norm() ) << '"'
	   << svgProperties( transform ) << ' '
	   << " transform=\"rotate(" << -angle << ", " 
	   << transform.mapX( _points[0].x ) << ", " << transform.mapY( _points[0].y ) << ") \" "
	   << " />" << std::endl;
  }
}

Point
Ellipse::center() const {
  return _center;
}

Shape &
Ellipse::rotate( double angle, const Point & center )
{ 
  Point c( _center );
  Point e = (c + Point( _xRadius, 0 )).rotate( _angle, c );
  Point rc = c.getRotate( angle, center );
  Point re = e.getRotate( angle, center );
  Point axis = re - rc;
  _angle = atan( axis.y / axis.x );
  _center = rc;
  return *this;
}

Shape &
Ellipse::rotate( double angle )
{
  return rotate( angle, center() );
}

Shape &
Ellipse::translate( double dx, double dy )
{
  _center += Point( dx, dy );
  return *this;
}

Shape &
Ellipse::scale( double sx, double sy )
{
  if ( _angle != 0.0 ) {
    Point cx = Point( _xRadius, 0 ).rotate( _angle );
    Point cy = Point( 0, _yRadius ).rotate( _angle );
    cx.x *= sx;
    cx.y *= sy;
    cy.x *= sx;
    cy.y *= sy;
    _xRadius = cx.norm();
    //_yRadius = cy.norm();
    _angle = atan( cx.y / cx.x);
  } else {
    _xRadius *= sx;
    _yRadius *= sy;
  }
  if ( _xRadius != _yRadius ) _circle = false;
  return *this;
}

void
Ellipse::flushPostscript( std::ostream & stream,
			 const TransformEPS & transform ) const
{
  double yScale = _yRadius / _xRadius;
  
  if ( filled() ) {
    stream << "gs " << transform.mapX( _center.x ) << " " << transform.mapY( _center.y ) << " tr";
    if ( ! _circle ) stream << " " << 1.0 << " " << yScale << " sc";
    if ( _angle != 0.0 ) stream << " " << (_angle*180/M_PI) << " rot ";
    stream << " n " << transform.scale( _xRadius ) << " 0 m " 
	   << " 0 0 " << transform.scale( _xRadius ) << " 0.0 360.0 arc ";
    stream << " " << _fillColor.postscript() << " srgb";
    stream << " fill gr" << std::endl;  
  }
  
  if ( _penColor != Color::none ) {
    stream << postscriptProperties() << "\n";
    stream << "gs " << transform.mapX( _center.x ) << " " << transform.mapY( _center.y ) << " tr";
    if ( ! _circle ) stream << " " << 1.0 << " " << yScale << " sc";
    if ( _angle != 0.0 ) stream << " " << (_angle*180/M_PI) << " rot ";
    stream << " n " << transform.scale( _xRadius ) << " 0 m " 
	   << " 0 0 " << transform.scale( _xRadius ) << " 0.0 360.0 arc ";
    stream << " " << _penColor.postscript() << " srgb";
    stream << " stroke gr" << std::endl;  
  }
}

void
Ellipse::flushFIG( std::ostream & stream,
		  const TransformFIG & transform,
		  std::map<Color,int> & colormap ) const
{
  // Ellipse, Sub type, Line style, Thickness
  if ( _circle )
    stream << "1 3 0 ";
  else
    stream << "1 1 0 ";
  stream << ( _penColor.valid()?transform.mapWidth( _lineWidth ):0 ) << " ";
  // Pen color, Fill color
  stream << colormap[ _penColor ] << " " << colormap[ _fillColor ] << " ";
  // Depth, Pen style, Area fill, Style val, Direction, angle
  if ( filled() )
    stream << transform.mapDepth( _depth ) << " -1 20 0.000 1 " << _angle << " ";
  else
    stream << transform.mapDepth( _depth ) << " -1 -1 0.000 1 " << _angle << " ";
  stream << static_cast<int>( transform.mapX( _center.x ) ) << " " 
	 << static_cast<int>( transform.mapY( _center.y ) ) << " " 
	 << static_cast<int>( transform.scale( _xRadius ) ) << " " 
	 << static_cast<int>( transform.scale( _yRadius ) ) << " " 
	 << static_cast<int>( transform.mapX( _center.x ) ) << " " 
	 << static_cast<int>( transform.mapY( _center.y ) ) << " " 
	 << static_cast<int>( transform.mapX( _center.x ) + transform.scale( _xRadius ) ) << " "
	 << static_cast<int>( transform.mapY( _center.y ) ) << "\n";
}

void
Ellipse::flushSVG( std::ostream & stream,
		  const TransformSVG & transform ) const
{
  stream << "<ellipse cx=\"" << transform.mapX( _center.x ) << '"'
	 << " cy=\"" << transform.mapY( _center.y ) << '"'
	 << " rx=\"" << transform.scale( _xRadius ) << '"'
	 << " ry=\"" << transform.scale( _yRadius ) << '"'
    	 << svgProperties( transform ) ;
  if ( _angle != 0.0 ) {
    stream << " transform=\"rotate( " 
	   << -(_angle*180/M_PI) << ", " 
	   << transform.mapX( _center.x ) << ", "
	   << transform.mapY( _center.y ) << " )\" "; 
  }
  stream << " />" << std::endl;
}

Rect
Ellipse::boundingBox() const
{
  if ( _angle == 0.0 )
    return Rect( _center.x - _xRadius, _center.y + _yRadius,
		 2 * _xRadius, 2 * _yRadius ); 
    
  double angleXmax = -atan( (_yRadius/_xRadius)*(tan(_angle) ) );
  double angleXmin = -atan( (_yRadius/_xRadius)*(tan(_angle) ) ) + M_PI;
  double angleYmax =  atan( (_yRadius/_xRadius)*(1/tan(_angle) ) );
  double angleYmin =  M_PI + atan( (_yRadius/_xRadius)*(1/tan(_angle) ) );
  
  if ( _angle < 0.0 ) { 
    angleYmax += M_PI;
    angleYmin -= M_PI;
  }

  return Rect( _center.x + _xRadius*cos(angleXmin)*cos(_angle) - _yRadius*sin(angleXmin)*sin(_angle),
	       _center.y + _xRadius*cos(angleYmax)*sin(_angle) + _yRadius*sin(angleYmax)*cos(_angle),
	       ( _xRadius*cos(angleXmax)*cos(_angle) - _yRadius*sin(angleXmax)*sin(_angle) ) - 
	       ( _xRadius*cos(angleXmin)*cos(_angle) - _yRadius*sin(angleXmin)*sin(_angle) ),
	       ( _xRadius*cos(angleYmax)*sin(_angle) + _yRadius*sin(angleYmax)*cos(_angle) ) -
	       ( _xRadius*cos(angleYmin)*sin(_angle) + _yRadius*sin(angleYmin)*cos(_angle) ) );
}


Point
Circle::center() const {
  return _center;
}

Shape &
Circle::rotate( double angle, const Point & center )
{
  if ( _circle ) {
    if ( center == _center ) return *this;
    _center.rotate( angle, center );
    return *this;
  }
  return Ellipse::rotate( angle, center );
}

Shape &
Circle::rotate( double angle )
{
  return rotate( angle, center() );
}

Shape &
Circle::translate( double dx, double dy )
{
  _center += Point( dx, dy );
  return *this;
}

Shape &
Circle::scale( double sx, double sy )
{
  Ellipse::scale( sx, sy );
  return *this;
}

void
Circle::flushSVG( std::ostream & stream,
		  const TransformSVG & transform ) const
{
  if ( ! _circle ) 
    Ellipse::flushSVG( stream, transform );
  else {
    stream << "<circle cx=\"" << transform.mapX( _center.x ) << '"'
	   << " cy=\"" << transform.mapY( _center.y ) << '"'
	   << " r=\"" << transform.scale( _xRadius ) << '"'
	   << svgProperties( transform ) 
	   << " />" << std::endl;
  }      
}

Point
Polyline::center() const {
  std::vector<Point>::const_iterator i = _points.begin();
  std::vector<Point>::const_iterator end = _points.end();
  double f = 1.0 / _points.size();
  Point r(0,0);
  while ( i != end ) {
    r += f * (*i);
    ++i;
  }
  return r;
}

Shape &
Polyline::rotate( double angle, const Point & center )
{
  std::vector<Point>::iterator i = _points.begin();
  std::vector<Point>::iterator end = _points.end();
  while ( i != end ) {
    (*i).rotate( angle, center );
    ++i;
  }
  return *this;
}

Shape &
Polyline::rotate( double angle )
{
  return rotate( angle, center() );
}

Shape &
Polyline::translate( double dx, double dy )
{
  std::vector<Point>::iterator i = _points.begin();
  std::vector<Point>::iterator end = _points.end();
  Point delta( dx, dy );
  while ( i != end ) {
    (*i) += delta;
    ++i;
  }
  return *this;
}

Shape &
Polyline::scale( double sx, double sy )
{
  Point c = center();
  std::vector<Point>::iterator i = _points.begin();
  std::vector<Point>::iterator end = _points.end();
  while ( i != end ) {
    i->x *= sx;
    i->y *= sy;
    ++i;
  }
  Point delta = c - center();
  translate( delta.x, delta.y );
  return *this;  
}

void
Polyline::flushPostscript( std::ostream & stream,
			   const TransformEPS & transform ) const
{
  std::vector<Point>::const_iterator i = _points.begin();
  std::vector<Point>::const_iterator end = _points.end();
  
  if ( filled() ) {
    stream << "n " << transform.mapX( i->x ) << " " << transform.mapY( i->y ) << " m";
    ++i;
    while ( i != end ) {
      stream << " " << transform.mapX( i->x ) << " " << transform.mapY( i->y ) << " l";
      ++i;
    }
    if ( _closed ) stream << " cp";
    stream << " ";
    _fillColor.flushPostscript( stream );
    stream << _lineWidth << " slw ";
    stream << _lineCap << " slc ";
    stream << _lineJoin << " slj";
    stream << " fill" << std::endl;  
  }

  i =_points.begin();
  if ( _penColor != Color::none ) {
    stream << " " << postscriptProperties() << "\n";
    stream << "n " << transform.mapX( i->x ) << " " << transform.mapY( i->y ) << " m";
    ++i;
    while ( i != end ) {
      stream << " " << transform.mapX( i->x ) << " " << transform.mapY( i->y ) << " l";
      ++i;
    }
    if ( _closed ) stream << " cp";
    stream << " ";
    _penColor.flushPostscript( stream );
    stream << " stroke" << std::endl;
  }
}

void
Polyline::flushFIG( std::ostream & stream,
		    const TransformFIG & transform,
		    std::map<Color,int> & colormap ) const
{
  if ( _closed ) 
    stream << "2 3 0 ";
  else
    stream << "2 1 0 ";
  // Thickness
  stream << ( _penColor.valid()?transform.mapWidth( _lineWidth ):0 ) << " ";
  // Pen color
  stream << colormap[ _penColor ] << " ";
  // Fill color
  stream << colormap[ _fillColor ] << " ";
  // Depth
  stream << transform.mapDepth( _depth ) << " ";
  // Pen style
  stream <<  "-1 ";
  // Area fill, style val, join style, cap style, radius, f_arrow, b_arrow
  if ( filled() ) 
    stream << "20 0.000 " << _lineJoin << " " << _lineCap << " -1 0 0 ";
  else
    stream << "-1 0.000 " << _lineJoin << " " << _lineCap << " -1 0 0 ";
  // Number of points
  stream << _points.size() + _closed << std::endl;
  stream << "         ";

  std::vector<Point>::const_iterator i = _points.begin();
  std::vector<Point>::const_iterator end = _points.end();
  while ( i != end ) {
    stream << " " << static_cast<int>( transform.mapX( i->x ) )
	   << " " << static_cast<int>( transform.mapY( i->y ) );
    ++i;
  }
  if ( _closed ) { 
    stream << " " << static_cast<int>( transform.mapX( _points.begin()->x ) )
	   << " " << static_cast<int>( transform.mapY( _points.begin()->y ) );
  }  
  stream << std::endl;
}

void
Polyline::flushSVG( std::ostream & stream,
		    const TransformSVG & transform ) const
{
  std::vector<Point>::const_iterator i = _points.begin();
  std::vector<Point>::const_iterator end = _points.end();
  int count = 0;
  if ( _closed )
    stream << "<polygon ";
  else
    stream << "<polyline";
  
  stream << svgProperties( transform ) << std::endl;
  stream << "          points=\"";

  stream << transform.mapX( i->x ) << "," << transform.mapY( i->y );
  ++i;
  while ( i != end ) {
    stream << " " << transform.mapX( i->x ) << "," << transform.mapY( i->y );
    ++i;
    count = ( count + 1 ) % 6;
    if ( !count ) stream << "\n                  ";
  }
  stream << "\" />" << std::endl;
}

Rect
Polyline::boundingBox() const
{
  Rect rect;
  std::vector< Point >::const_iterator i = _points.begin();
  std::vector< Point >::const_iterator end = _points.end();
  rect.top = i->y;
  rect.left = i->x;
  rect.width = 0.0;
  rect.height = 0.0;
  ++i;
  while ( i != end ) { 
    if ( i->x < rect.left ) { 
      double dw = rect.left - i->x;
      rect.left = i->x;
      rect.width += dw;
    } else if ( i->x > rect.left + rect.width ) {
      rect.width = i->x - rect.left;
    }
    if ( i->y > rect.top ) { 
      double dh = i->y - rect.top;
      rect.top = i->y;
      rect.height += dh;
    } else if ( i->y < rect.top - rect.height ) {
      rect.height = rect.top - i->y;
    }
    ++i;
  }
  return rect;
}

GouraudTriangle::GouraudTriangle( const Point & p0, const Color & color0,
				  const Point & p1, const Color & color1,
				  const Point & p2, const Color & color2,
				  int subdivisions,
				  int depth )
  : Polyline( std::vector<Point>(), true, Color::none, Color::none,
	      0.0f, ButtCap, MiterJoin, depth ),
    _color0( color0 ), _color1( color1 ), _color2( color2 ), _subdivisions( subdivisions ) {
  _points.push_back( p0 );
  _points.push_back( p1 );
  _points.push_back( p2 );

  Shape::_fillColor.red( ( color0.red() + color1.red() + color2.red() ) / 3 );
  Shape::_fillColor.green( ( color0.green() + color1.green() + color2.green() ) / 3 );
  Shape::_fillColor.blue( ( color0.blue() + color1.blue() + color2.blue() ) / 3 );
}

GouraudTriangle::GouraudTriangle( const Point & p0, float brightness0,
				  const Point & p1, float brightness1,
				  const Point & p2, float brightness2,
				  const Color & _fillColor,
				  int subdivisions,
				  int depth )
  : Polyline( std::vector<Point>(), true, Color::none, Color::none,
	      0.0f, ButtCap, MiterJoin, depth ),
    _color0( _fillColor ), _color1( _fillColor ), _color2( _fillColor ), _subdivisions( subdivisions )
{
  _points.push_back( p0 );
  _points.push_back( p1 );
  _points.push_back( p2 );
  _color0.red( static_cast<unsigned char>( std::min( 255.0f, _color0.red() * brightness0 ) ) );
  _color0.green( static_cast<unsigned char>( std::min( 255.0f, _color0.green() * brightness0 ) ) );
  _color0.blue( static_cast<unsigned char>( std::min( 255.0f, _color0.blue() * brightness0 ) ) );
  _color1.red( static_cast<unsigned char>( std::min( 255.0f, _color1.red() * brightness1 ) ) );
  _color1.green( static_cast<unsigned char>( std::min( 255.0f, _color1.green() * brightness1 ) ) );
  _color1.blue( static_cast<unsigned char>( std::min( 255.0f, _color1.blue() * brightness1 ) ) );
  _color2.red( static_cast<unsigned char>( std::min( 255.0f, _color2.red() * brightness2 ) ) );
  _color2.green( static_cast<unsigned char>( std::min( 255.0f, _color2.green() * brightness2 ) ) );
  _color2.blue( static_cast<unsigned char>( std::min( 255.0f, _color2.blue() * brightness2 ) ) );
  
  Shape::_fillColor.red( ( _color0.red() + _color1.red() + _color2.red() ) / 3 );
  Shape::_fillColor.green( ( _color0.green() + _color1.green() + _color2.green() ) / 3 );
  Shape::_fillColor.blue( ( _color0.blue() + _color1.blue() + _color2.blue() ) / 3 );
}

Point
GouraudTriangle::center() const {
  return ( _points[0] + _points[1] + _points[2] ) / 3.0;
}

Shape &
GouraudTriangle::rotate( double angle, const Point & center )
{
  _points[0].rotate( angle, center );
  _points[1].rotate( angle, center );
  _points[2].rotate( angle, center );
  return *this;
}

Shape &
GouraudTriangle::rotate( double angle )
{
  return rotate( angle, center() );
}

void
GouraudTriangle::flushPostscript( std::ostream & stream,
				  const TransformEPS & transform ) const
{
  if ( ! _subdivisions ) {
    Polyline::flushPostscript( stream, transform );
    return;
  }
  const Point & p0 = _points[0];
  const Point & p1 = _points[1];
  const Point & p2 = _points[2];  
  Point p01( 0.5*(p0.x+p1.x), 0.5*(p0.y+p1.y) );
  Color c01( (_color0.red() + _color1.red())/2, 
	     (_color0.green() + _color1.green())/2, 
	     (_color0.blue() + _color1.blue())/2 );
  Point p12( 0.5*(p1.x+p2.x), 0.5*(p1.y+p2.y) );
  Color c12( (_color1.red() + _color2.red())/2, 
	     (_color1.green() + _color2.green())/2, 
	     (_color1.blue() + _color2.blue())/2 );
  Point p20( 0.5*(p2.x+p0.x), 0.5*(p2.y+p0.y) );
  Color c20( (_color2.red() + _color0.red())/2, 
	     (_color2.green() + _color0.green())/2, 
	     (_color2.blue() + _color0.blue())/2 );
  GouraudTriangle( p0, _color0, p20, c20, p01, c01, _subdivisions - 1, _depth ).flushPostscript( stream, transform );
  GouraudTriangle( p1, _color1, p01, c01, p12, c12, _subdivisions - 1, _depth ).flushPostscript( stream, transform );
  GouraudTriangle( p2, _color2, p20, c20, p12, c12, _subdivisions - 1, _depth ).flushPostscript( stream, transform );
  GouraudTriangle( p01, c01, p12, c12, p20, c20,  _subdivisions - 1, _depth ).flushPostscript( stream, transform );
}

void
GouraudTriangle::flushFIG( std::ostream & stream,
			   const TransformFIG & transform,
			   std::map<Color,int> & colormap ) const
{
  Polyline::flushFIG( stream, transform, colormap );
}

void
GouraudTriangle::flushSVG( std::ostream & stream,
			   const TransformSVG & transform ) const
{
  if ( ! _subdivisions ) {
    Polyline::flushSVG( stream, transform );
    return;
  }
  const Point & p0 = _points[0];
  const Point & p1 = _points[1];
  const Point & p2 = _points[2];  
  Point p01( 0.5*(p0.x+p1.x), 0.5*(p0.y+p1.y) );
  Color c01( (_color0.red() + _color1.red())/2, 
	     (_color0.green() + _color1.green())/2, 
	     (_color0.blue() + _color1.blue())/2 );
  Point p12( 0.5*(p1.x+p2.x), 0.5*(p1.y+p2.y) );
  Color c12( (_color1.red() + _color2.red())/2, 
	     (_color1.green() + _color2.green())/2, 
	     (_color1.blue() + _color2.blue())/2 );
  Point p20( 0.5*(p2.x+p0.x), 0.5*(p2.y+p0.y) );
  Color c20( (_color2.red() + _color0.red())/2, 
	     (_color2.green() + _color0.green())/2, 
	     (_color2.blue() + _color0.blue())/2 );
  GouraudTriangle( p0, _color0, p20, c20, p01, c01, _subdivisions - 1, _depth ).flushSVG( stream, transform );
  GouraudTriangle( p1, _color1, p01, c01, p12, c12, _subdivisions - 1, _depth ).flushSVG( stream, transform );
  GouraudTriangle( p2, _color2, p20, c20, p12, c12, _subdivisions - 1, _depth ).flushSVG( stream, transform );
  GouraudTriangle( p01, c01, p12, c12, p20, c20,  _subdivisions - 1, _depth ).flushSVG( stream, transform ); 
}

Point
Text::center() const {
  return _position;
}

Shape &
Text::rotate( double /*angle*/, const Point & /*center*/ )
{
  // Not implemented yet.
  return *this;
}

Shape &
Text::rotate( double angle )
{
  return rotate( angle, center() );
}
  
Shape &
Text::translate( double dx, double dy )
{
  _position += Point( dx, dy );
  return *this;
}

Shape &
Text::scale( double sx, double sy )
{
  _xScale = sx;
  _yScale = sy;
  return *this;
}

void
Text::flushPostscript( std::ostream & stream,
		       const TransformEPS & transform ) const
{
  stream << "/" << _font << " ff " << _size << " scf sf";
  stream << " " << transform.mapX( _position.x ) << " " << transform.mapY( _position.y ) << " m";
  stream << " (" << _text << ")" 
	 << " " << _penColor.postscript() << " srgb"
	 << " sh" << std::endl;
}

void
Text::flushFIG( std::ostream & stream,
		const TransformFIG & transform,
		std::map<Color,int> & colormap ) const
{
  int i = 0;
  for ( i = 0 ; XFigPostscriptFontnames[ i ] && strcmp( XFigPostscriptFontnames[ i ], 
							_font.c_str() ); ++i );
  if ( ! XFigPostscriptFontnames[ i ] ) i = 0;
  stream << "4 0 " ;
  // Color, depth, unused, Font
  stream << colormap[ _penColor ] <<  " " << transform.mapDepth( _depth ) << " -1 " << i << " ";
  // Font size, Angle, Font flags
  stream << _size << " 0.000 4 ";
  // Height
  stream << static_cast<int>( _size * 135 / 12.0 ) << " ";
  // Width
  stream << static_cast<int>( _text.size() * _size * 135 / 12.0 ) << " ";
  // x y 
  stream << static_cast<int>( transform.mapX( _position.x ) ) << " "
	 << static_cast<int>( transform.mapY( _position.y ) ) << " ";
  stream << _text << "\\001\n";
}

void
Text::flushSVG( std::ostream & stream,
		       const TransformSVG & transform ) const
{
  stream << "<text x=\"" << transform.mapX( _position.x )
	 << "\" y=\"" << transform.mapY( _position.y ) << "\" "
	 << " font-family=\"" << _font << "\""
	 << " font-size=\"" << _size << "\""
	 << " fill=\"" << _penColor.svg() 
	 << _fillColor.svgAlpha( " fill" )
	 << _penColor.svgAlpha( " stroke" )
	 << "\" >" << std::endl
	 << _text << std::endl
	 << "</text>" << std::endl;
}

Rect
Text::boundingBox() const
{
  return Rect( _position.x, _position.y, 0, 0 );
}

} // namespace BoardLib
