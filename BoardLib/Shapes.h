/* -*- mode: c++ -*- */
/**
 * @file   Shape.h
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  
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
#ifndef _BOARD_SHAPES_H_
#define _BOARD_SHAPES_H_

#include "Point.h"
#include "Rect.h"
#include "Color.h"
#include "Transforms.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2         1.57079632679489661923
#endif

namespace BoardLib {

/**
 * Shape structure.
 * @brief Abstract structure for a 2D shape.
 */
struct Shape {

  enum LineCap { ButtCap = 0, RoundCap, SquareCap };
  enum LineJoin { MiterJoin = 0, RoundJoin, BevelJoin };

  /** 
   * Shape constructor.
   * 
   * @param penColor The pen color of the shape.
   * @param fillColor The fill color of the shape.
   * @param lineWidth The line thickness.
   * @param depth The depth of the shape.
   */
  Shape( Color penColor, Color fillColor,
	 float lineWidth, const LineCap cap, const LineJoin join,
	 int depth )
    : _depth( depth ), _penColor( penColor ), _fillColor( fillColor ), 
      _lineWidth( lineWidth ), _lineCap( cap ), _lineJoin( join ) { }

  /** 
   * Shape destructor.
   */
  virtual ~Shape() { }

  /** 
   * Return a copy of the shape.
   * 
   * @return 
   */
  virtual Shape * clone() const = 0;
  
  /** 
   * Checks whether a shape is filled with a color or not.
   * 
   * @return true if the shape is filled.
   */
  inline bool filled() const { return _fillColor != Color::none; }
  
  /** 
   * Returns the gravity center of the shape.
   * 
   * @return The center of the shape.
   */
  virtual Point center() const = 0;
  
  /** 
   * Rotate the shape around a given center of rotation.
   * 
   * @param angle The rotation angle in radian.
   * @param center The center of rotation.
   * 
   * @return A reference to the shape itself.
   */
  virtual Shape & rotate( double angle, const Point & center ) = 0;

  /** 
   * Rotate the shape around its center.
   * 
   * @param angle The rotation angle in radian.
   * 
   * @return A reference to the shape itself.
   */
  virtual Shape & rotate( double angle ) = 0;

  /** 
   * Rotate the shape around a given center of rotation.
   * 
   * @param angle The rotation angle in degree.
   * @param center The center of rotation.
   * 
   * @return A reference to the shape itself.
   */
  inline Shape & rotateDeg( double angle, const Point & center );
  
  /** 
   * Rotate the shape around its center.
   * 
   * @param angle The rotation angle in degree.
   * @param center The center of rotation.
   * 
   * @return A reference to the shape itself.
   */
  inline Shape & rotateDeg( double angle );

  /** 
   * Translate the shape by a given offset.
   * 
   * @param dx The x offset.
   * @param dy The y offset.
   * 
   * @return A reference to the shape itself.
   */
  virtual Shape & translate( double dx, double dy ) = 0;

  /** 
   * Scale the shape along the x an y axis.
   * 
   * @param sx The scale factor along the x axis.
   * @param sy The scale factor along the y axis.
   * 
   * @return The shape itself.
   */
  virtual Shape & scale( double sx, double sy ) = 0;

  /** 
   * Writes the EPS code of the shape in a stream according
   * to a transform.
   * 
   * @param stream The output stream.
   * @param transform A 2D transform to be applied.
   */
  virtual void flushPostscript( std::ostream & stream,
				const TransformEPS & transform ) const = 0;

  /** 
   * Writes the FIG code of the shape in a stream according
   * to a transform.
   * 
   * @param stream The output stream.
   * @param transform A 2D transform to be applied.
   */
  virtual void flushFIG( std::ostream & stream,
			 const TransformFIG & transform,
			 std::map<Color,int> & colormap ) const = 0;

  /** 
   * Writes the SVG code of the shape in a stream according
   * to a transform.
   * 
   * @param stream The output stream.
   * @param transform A 2D transform to be applied.
   */
  virtual void flushSVG( std::ostream & stream,
			 const TransformSVG & transform ) const = 0;
  
  /** 
   * Returns the bounding box of the figure.
   *
   * @return The rectangle of the bounding box.
   */
  virtual Rect boundingBox() const = 0;


  inline int depth() const;
  virtual void depth( int );
  inline const Color & penColor() const;
  inline const Color & fillColor() const;
  
protected:

  int _depth;    		/**< The depth of the shape. */
  Color _penColor;		/**< The color of the shape. */
  Color _fillColor;		/**< The color of the shape. */
  float _lineWidth;		/**< The line thickness. */
  LineCap _lineCap;		/**< The linecap attribute. (The way line terminates.) */
  LineJoin _lineJoin;		/**< The linejoin attribute. (The shape of line junctions.) */

  /** 
   * Return a string of the svg properties lineWidth, opacity, penColor, fillColor,
   * lineCap, and lineJoin.
   * 
   * @return A string of the properties suitable for inclusion in an svg tag.
   */
  std::string svgProperties( const TransformSVG & transform ) const;

  
  /** 
   * Return a string of the properties lineWidth, penColor, lineCap, and lineJoin
   * as Postscript commands.
   * @return A string of the Postscript commands.
   */
  std::string postscriptProperties() const;

};

inline int
Shape::depth() const
{
  return _depth;
}

inline const Color &
Shape::penColor() const
{
  return _penColor;
}

const Color &
Shape::fillColor() const
{
  return _fillColor;
}

Shape &
Shape::rotateDeg( double angle, const Point & center )
{
  return rotate( angle * ( M_PI / 180.0 ), center ); 
}

Shape &
Shape::rotateDeg( double angle )
{
  return rotate( angle * ( M_PI / 180.0 ), center() ); 
}

/**
 * The line structure.
 * @brief A line between two points.
 */
struct Line : public Shape { 
  
  /** 
   * Constructs a line.
   * 
   * @param x1 First coordinate of the start point.
   * @param y1 Second coordinate of the start point.
   * @param x2 First coordinate of the end point.
   * @param y2 Second coordinate of the end point.
   * @param color The color of the line.
   * @param lineWidth The line thickness.
   * @param depth The depth of the line.
   */
  Line( double x1, double y1, double x2, double y2, 
	Color color, 
	float lineWidth, const LineCap cap = ButtCap, const LineJoin join = MiterJoin,
	int depth = -1 )
    : Shape( color, Color::none, lineWidth, cap, join, depth ),
      _x1( x1 ), _y1( y1 ), _x2( x2 ), _y2( y2 ) { }

  Point center() const;

  Shape & rotate( double angle, const Point & center );

  Shape & rotate( double angle );

  Shape & translate( double dx, double dy );

  Shape & scale( double sx, double sy );
  
  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;
  
  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Rect boundingBox() const;

  Shape * clone() const { return new Line(*this); }

protected:
  double _x1;			/**< First coordinate of the start point. */
  double _y1;			/**< Second coordinate of the start point. */
  double _x2; 			/**< First coordinate of the end point. */
  double _y2;			/**< Second coordinate of the end point. */
};

/**
 * The arrow structure.
 * @brief A line between two points with an arrow at one extremity.
 */
struct Arrow : public Line { 

  /** 
   * Constructs an arrow.
   * 
   * @param x1 First coordinate of the start point.
   * @param y1 Second coordinate of the start point.
   * @param x2 First coordinate of the end point.
   * @param y2 Second coordinate of the end point.
   * @param penColor The color of the line.
   * @param fillColor The fill color of the sharp end.
   * @param lineWidth The line thickness.
   * @param depth The depth of the line.
   */
  Arrow( double x1, double y1, double x2, double y2,
	 Color penColor, Color fillColor,
	 float lineWidth, const LineCap cap = ButtCap, const LineJoin join = MiterJoin,
	 int depth = -1 )
    : Line( x1, y1, x2, y2, penColor, lineWidth, cap, join, depth ) {
    Shape::_fillColor = fillColor;
  }
  
  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;
  
  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;
  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Shape * clone() const { return new Arrow(*this); }
};

/**
 * The polyline structure.
 * @brief A polygonal line described by a series of 2D points.
 */
struct Polyline : public Shape { 
  Polyline( const std::vector<Point> & points, 
	    bool closed,
	    Color penColor, Color fillColor,
	    float lineWidth, const LineCap cap = ButtCap, const LineJoin join = MiterJoin,
	    int depth = -1 )
    : Shape( penColor, fillColor, lineWidth, cap, join, depth ),
      _points( points ), _closed( closed ) { }
	    
  Point center() const;

  Shape & rotate( double angle, const Point & center );

  Shape & rotate( double angle );

  Shape & translate( double dx, double dy );

  Shape & scale( double sx, double sy );

  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;

  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Rect boundingBox() const;

  Shape * clone() const { return new Polyline(*this); }

protected:
  std::vector<Point> _points;
  bool _closed;
};

/**
 * The rectangle structure.
 * @brief A rectangle.
 */
struct Rectangle : public Polyline {

  Rectangle( double x, double y, double width, double height,
	     Color penColor, Color fillColor,
	     float lineWidth, const LineCap cap = ButtCap, const LineJoin join = MiterJoin,
	     int depth = -1 )
    : Polyline( std::vector<Point>(), true, penColor, fillColor, lineWidth, cap, join, depth ) {
    _points.push_back( Point( x, y ) );
    _points.push_back( Point( x + width, y ) );
    _points.push_back( Point( x + width, y - height ) );
    _points.push_back( Point( x, y - height ) );
  }
  
  Rectangle( const Rect & rect,
	     Color penColor, Color fillColor,
	     float lineWidth, const LineCap cap = ButtCap, const LineJoin join = MiterJoin,
	     int depth = -1 )
    : Polyline( std::vector<Point>(), true, penColor, fillColor, lineWidth, cap, join, depth ) {
    _points.push_back( Point( rect.left, rect.top ) );
    _points.push_back( Point( rect.left + rect.width, rect.top ) );
    _points.push_back( Point( rect.left + rect.width, rect.top - rect.height ) );
    _points.push_back( Point( rect.left, rect.top - rect.height ) );
  }

  double x() const { return _points[0].x; }
  double y() const { return _points[0].y; }
  double width() { return (_points[1] - _points[0]).norm(); }
  double height() { return (_points[0] - _points[3]).norm(); }
  
  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Shape * clone() const { return new Rectangle(*this); }
};

/**
 * The GouraudTriangle structure.
 * @brief A triangle with shaded filling according to colors given for each vertex. 
 */
struct GouraudTriangle : public Polyline {

  GouraudTriangle( const Point & p0, const Color & color0,
		   const Point & p1, const Color & color1,
		   const Point & p2, const Color & color2,
		   int subdivisions,
		   int depth = -1 );

  GouraudTriangle( const Point & p0, float brightness0,
		   const Point & p1, float brightness1,
		   const Point & p2, float brightness2,
		   const Color & fillColor,
		   int subdivisions,
		   int depth = -1 );

  Point center() const;

  Shape & rotate( double angle, const Point & center );  
  Shape & rotate( double angle );

  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;

  /** 
   * Sends the Triangle to a FIG file format stream.
   * <p><b>Warning!</b> Because shading would generally require
   * more colors in the colormap than allowed by the FIG file format, 
   * rendering a Gouraud triangle in an XFig file is the same as rendering
   * a simple triangle whose filling color is the average of the vertex colors.
   * 
   * @param stream 
   * @param transform 
   * @param Color 
   * @param colormap 
   */
  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Shape * clone() const { return new GouraudTriangle(*this); }
protected:
  Color _color0;
  Color _color1;
  Color _color2;
  int _subdivisions;
};

/**
 * The ellipse structure.
 * @brief An ellipse.
 */
struct Ellipse : public Shape {
  
  Ellipse( double x, double y, 
	   double xRadius, double yRadius, 
	   Color penColor, Color fillColor,
	   float lineWidth, int depth = -1 )
    : Shape( penColor, fillColor, lineWidth,  ButtCap, MiterJoin, depth ),
      _center( x, y ), _xRadius( xRadius ), _yRadius( yRadius ), _angle( 0.0 ),
      _circle( false ) {
    while ( _angle > M_PI_2 ) _angle -= M_PI;
    while ( _angle < -M_PI_2 ) _angle += M_PI;
  }

  Point center() const;

  Shape & rotate( double angle, const Point & center );
  Shape & rotate( double angle );
  
  Shape & translate( double dx, double dy );

  Shape & scale( double sx, double sy );
  
  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;

  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Rect boundingBox() const;

  Shape * clone() const { return new Ellipse(*this); }
 
protected:
  Point _center;
  double _xRadius; 
  double _yRadius;
  double _angle;
  bool _circle;
};

/**
 * The circle structure.
 * @brief A circle.
 */
struct Circle : public Ellipse {

  Circle( double x, double y, double radius, 
	  Color penColor, Color fillColor,
	  float lineWidth,
	  int depth = -1 )
    : Ellipse( x, y, radius, radius, penColor, fillColor, lineWidth, depth )
  { _circle = true; }

  Point center() const;

  Shape & rotate( double angle, const Point & center );

  Shape & rotate( double angle );
  
  Shape & translate( double dx, double dy );

  Shape & scale( double sx, double sy );

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Shape * clone() const { return new Circle(*this); }
};

/**
 * The text structure.
 * @brief A piece of text.
 */
struct Text : public Shape {
  
  Text( double x, double y,
	const std::string & text, const std::string & font, float size,
	Color color, int depth = -1 )
    : Shape( color, Color::none, 1.0, ButtCap, MiterJoin, depth ),
      _position( x, y ), _text( text ), _font( font ), _size( size ),
      _xScale( 1.0 ), _yScale( 1.0 ) { }
  
  Point center() const;

  Shape & rotate( double angle, const Point & center );

  Shape & rotate( double angle );
  
  Shape & translate( double dx, double dy );

  Shape & scale( double sx, double sy );
  
  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;

  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Rect boundingBox() const;

  Shape * clone() const { return new Text(*this); }

protected:
  Point _position;
  std::string _text;
  std::string _font;
  float _size;
  double _xScale;
  double _yScale;
};

/** 
 * Compares two shapes according to their depths.
 * 
 * @param s1 A pointer to a first shape.
 * @param s2 A pointer to a second shape.
 * 
 * @return 
 */
bool shapeGreaterDepth( const Shape *s1, const Shape *s2 );

} // namespace BoardLib 



#endif /* _SHAPE_H_ */

