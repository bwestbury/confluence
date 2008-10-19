/* -*- mode: c++ -*- */
/**
 * @file   Board.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  
 * @copyright
 */

#include "Board.h"
#include "Point.h"
#include "Rect.h"
#include "Shapes.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <map>
#include <algorithm>
#include <cstdio>
#include <algorithm>

namespace BoardLib {

const char * XFigPostscriptFontnames[] = {
  "Times-Roman",
  "Times-Italic",
  "Times-Bold",
  "Times-Bold-Italic",
  "AvantGarde-Book",
  "AvantGarde-Book-Oblique",
  "AvantGarde-Demi",
  "AvantGarde-Demi-Oblique",
  "Bookman-Light",
  "Bookman-Light-Italic",
  "Bookman-Demi",
  "Bookman-Demi-Italic",
  "Courier",
  "Courier-Oblique",
  "Courier-Bold",
  "Courier-Bold-Oblique",
  "Helvetica",
  "Helvetica-Oblique",
  "Helvetica-Bold",
  "Helvetica-Bold-Oblique",
  "Helvetica-Narrow",
  "Helvetica-Narrow-Oblique",
  "Helvetica-Narrow-Bold",
  "Helvetica-Narrow-Bold-Oblique",
  "New-Century-Schoolbook-Roman",
  "New-Century-Schoolbook-Italic",
  "New-Century-Schoolbook-Bold",
  "New-Century-Schoolbook-Bold-Italic",
  "Palatino-Roman",
  "Palatino-Italic",
  "Palatino-Bold",
  "Palatino-Bold-Italic",
  "Symbol",
  "Zapf-Chancery-Medium-Italic",
  "Zapf-Dingbats",
  NULL
};

Board::State::State()
{
  penColor = Color::Black;
  fillColor =  Color::none;
  lineWidth = 0.5;
  lineCap = Shape::ButtCap;
  lineJoin = Shape::MiterJoin;
  font = std::string( "Times-Roman" );
  fontSize = 11.0;
}

Board::Board( const Color & backgroundColor )
  : _backgroundColor( backgroundColor )
{
}

Board::Board( const Board & other )
  :ShapeList( other ),
   _state( other._state ),
   _backgroundColor( other._backgroundColor )
{
}

Board &
Board::operator=( const Board & other )
{
  free();
  if ( ! other._shapes.size() ) return (*this);  
  _shapes.resize( other._shapes.size(), 0 );
  std::vector<Shape*>::iterator t = _shapes.begin();
  std::vector<Shape*>::const_iterator i = other._shapes.begin();
  std::vector<Shape*>::const_iterator end = other._shapes.end();
  while ( i != end ) {
    *t = (*i)->clone();
    ++i; ++t;
  }  
  return *this;
}

Board::~Board()
{

}

void
Board::clear( const Color & color )
{
  ShapeList::clear();
  _backgroundColor = color;
}

Board &
Board::setPenColorRGBi( unsigned char red,
			unsigned char green,
			unsigned char blue, 
			unsigned char alpha )
{
  _state.penColor.setRGBi( red, green, blue, alpha );
  return *this;
}

Board &
Board::setPenColorRGBf(  float red,
			 float green,
			 float blue, 
			 float alpha )
{
  _state.fillColor.setRGBf( red, green, blue, alpha );
  return *this;
}

Board &
Board::setPenColor( const Color & color )
{
  _state.penColor = color;
  return *this;
}

Board &
Board::setFillColorRGBi( unsigned char red,
			 unsigned char green,
			 unsigned char blue,
			 unsigned char alpha )
{
  _state.fillColor.setRGBi( red, green, blue, alpha );
  return *this;
}

Board &
Board::setFillColorRGBf( float red, float green, float blue, float alpha )
{
  _state.fillColor.setRGBf( red, green, blue, alpha );
  return *this;
}

Board &
Board::setFillColor( const Color & color )
{
  _state.fillColor = color;
  return *this;
}

Board &
Board::setLineWidth( float width )
{
  _state.lineWidth = width;
  return *this;
}

Board &
Board::setFont( std::string font, float fontSize )
{
  _state.font = font;
  _state.fontSize = fontSize;
  return *this;
}

Board &
Board::setFontSize( float fontSize )
{
  _state.fontSize = fontSize;
  return *this;
}

void
Board::backgroundColor( const Color & color )
{
  _backgroundColor = color;
}

void
Board::drawLine( float x1, float y1, float x2, float y2, 
		 int depth /* = -1 */  )
{
  if ( depth != -1 ) 
    _shapes.push_back( new Line( x1, y1, x2, y2, _state.penColor, _state.lineWidth, _state.lineCap, _state.lineJoin, depth ) );
  else
    _shapes.push_back( new Line( x1, y1, x2, y2, _state.penColor, _state.lineWidth, _state.lineCap, _state.lineJoin, _nextDepth-- ) );
}

void
Board::drawArrow( float x1, float y1, float x2, float y2, 
		  bool filled /* = false */,
		  int depth /* = -1 */  )
{
  if ( depth != -1 )
    _shapes.push_back( new Arrow( x1, y1, x2, y2,
				  _state.penColor, filled ? _state.penColor : Color::none,
				  _state.lineWidth, _state.lineCap, _state.lineJoin, depth ) );
  else
    _shapes.push_back( new Arrow( x1, y1, x2, y2,
				  _state.penColor, filled ? _state.penColor : Color::none,
				  _state.lineWidth, _state.lineCap, _state.lineJoin, _nextDepth-- ) );
}

void
Board::drawRectangle( float x, float y, 
		      float width, float height,
		      int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Rectangle( x, y, width, height, 
				    _state.penColor, _state.fillColor,
				    _state.lineWidth, _state.lineCap, _state.lineJoin, d ) );
}

void
Board::fillRectangle( float x, float y,
		      float width, float height,
		      int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Rectangle( x, y, width, height,
				    Color::none, _state.penColor,
				    0.0f, _state.lineCap, _state.lineJoin,
				    d ) );
}

void
Board::drawCircle( float x, float y, float radius,
		   int depth /* = -1 */  )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Circle( x, y, radius, 
				 _state.penColor, _state.fillColor, _state.lineWidth, d ) );
}

void
Board::fillCircle( float x, float y, float radius,
		   int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Circle( x, y, radius, 
				 Color::none, _state.penColor, 0.0f, d ) );
}

void
Board::drawEllipse( float x, float y,
		    float xRadius, float yRadius,
		    int depth /* = -1 */  )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Ellipse( x, y, xRadius, yRadius,
				  _state.penColor, _state.fillColor, _state.lineWidth,
				  d ) );
}

void
Board::fillEllipse( float x, float y, 
		    float xRadius, float yRadius,
		    int depth /* = -1 */ )
{
  int d = depth ? depth : _nextDepth--;
  _shapes.push_back( new Ellipse( x, y, xRadius, yRadius,
				  Color::none, _state.penColor,
				  0.0f, d ) );
}

void
Board::drawPolyline( const std::vector<Point> & points,
		     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Polyline( points, false, _state.penColor, _state.fillColor,
				   _state.lineWidth, _state.lineCap, _state.lineJoin,
				   d ) );
}

void
Board::drawClosedPolyline( const std::vector<Point> & points,
			     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Polyline( points, true, _state.penColor, _state.fillColor,
				   _state.lineWidth, _state.lineCap, _state.lineJoin,
				   d ) );
}

void
Board::fillPolyline( const std::vector<Point> & points,
		       int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Polyline( points, true, Color::none, _state.penColor,
				   0.0f, _state.lineCap, _state.lineJoin,
				   d ) );
}

void
Board::drawTriangle( float x1, float y1, 
		     float x2, float y2, 
		     float x3, float y3, 
		     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  std::vector<Point> points;
  points.push_back( Point( x1, y1 ) );
  points.push_back( Point( x2, y2 ) );
  points.push_back( Point( x3, y3 ) );
  _shapes.push_back( new Polyline( points, true, _state.penColor, _state.fillColor,
				   _state.lineWidth, _state.lineCap, _state.lineJoin,
				   d ) );
}

void
Board::drawTriangle( const Point & p1,
		     const Point & p2, 
		     const Point & p3, 
		     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  std::vector<Point> points;
  points.push_back( p1 );
  points.push_back( p2 );
  points.push_back( p3 );
  _shapes.push_back( new Polyline( points, true, _state.penColor, _state.fillColor,
				   _state.lineWidth, _state.lineCap, _state.lineJoin,
				   d ) );  
}

void
Board::fillTriangle( float x1, float y1, 
		     float x2, float y2, 
		     float x3, float y3, 
		     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  std::vector<Point> points;
  points.push_back( Point( x1, y1 ) );
  points.push_back( Point( x2, y2 ) );
  points.push_back( Point( x3, y3 ) );
  _shapes.push_back( new Polyline( points, true, Color::none, _state.penColor,
				   0.0f, _state.lineCap, _state.lineJoin,
				   d ) );
}

void
Board::fillTriangle( const Point & p1,
		     const Point & p2, 
		     const Point & p3, 
		     int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  std::vector<Point> points;
  points.push_back( p1 );
  points.push_back( p2 );
  points.push_back( p3 );
  _shapes.push_back( new Polyline( points, true, Color::none, _state.penColor,
				   0.0f, _state.lineCap, _state.lineJoin,
				   d ) );  
}

void
Board::fillGouraudTriangle( const Point & p1,
			    const Color & color1,
			    const Point & p2,
			    const Color & color2,
			    const Point & p3,
			    const Color & color3,
			    unsigned char divisions,
			    int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new GouraudTriangle( p1, color1, p2, color2, p3, color3,
					  divisions, d ) );
}

void
Board::fillGouraudTriangle( const Point & p1,
			    const float brightness1,
			    const Point & p2,
			    const float brightness2,
			    const Point & p3,
			    const float brightness3,
			    unsigned char divisions,
			    int depth /* = -1 */ )
{
  Color color1( _state.penColor );
  Color color2( _state.penColor );
  Color color3( _state.penColor );
  color1.red( static_cast<unsigned char>( std::min( 255.0f, color1.red() * brightness1 ) ) );
  color1.green( static_cast<unsigned char>( std::min( 255.0f, color1.green() * brightness1 ) ) );
  color1.blue( static_cast<unsigned char>( std::min( 255.0f, color1.blue() * brightness1 ) ) );
  color2.red( static_cast<unsigned char>( std::min( 255.0f, color2.red() * brightness2 ) ) );
  color2.green( static_cast<unsigned char>( std::min( 255.0f, color2.green() * brightness2 ) ) );
  color2.blue( static_cast<unsigned char>( std::min( 255.0f, color2.blue() * brightness2 ) ) );
  color3.red( static_cast<unsigned char>( std::min( 255.0f, color3.red() * brightness3 ) ) );
  color3.green( static_cast<unsigned char>( std::min( 255.0f, color3.green() * brightness3 ) ) );
  color3.blue( static_cast<unsigned char>( std::min( 255.0f, color3.blue() * brightness3 ) ) );
  fillGouraudTriangle( p1, color1, p2, color2, p3, color3, divisions, depth );
}

void
Board::drawText( float x, float y, const char * text,
		 int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  _shapes.push_back( new Text( x, y, text, _state.font, _state.fontSize, _state.penColor, d ) );
}

void
Board::drawBoundingBox( int depth /* = -1 */ )
{
  int d = (depth!=-1) ? depth : _nextDepth--;
  Rect bbox = boundingBox();
  _shapes.push_back( new Rectangle( bbox.left,
				    bbox.top,
				    bbox.width,
				    bbox.height,
				    _state.penColor, _state.fillColor,
				    _state.lineWidth, _state.lineCap, _state.lineJoin,
				    d ) );
}
  
void
Board::saveEPS( const char * filename, float scale ) const
{
  std::ofstream file( filename );
  
  Rect bbox = boundingBox();
  TransformEPS transform;
  transform.setBoundingBox( bbox );
  if ( scale != 1.0 ) transform.furtherScale( scale );

  file << "%!PS-Adobe-2.0 EPSF-2.0" << std::endl;
  file << "%%Title: " << filename << std::endl;
  file << "%%Creator: Board library (C)2007 Sebastien Fourey" << std::endl;
  time_t t = time(0);
  file << "%%CreationDate: " << ctime(&t);
  file << "%%BoundingBox: " << std::setprecision( 8 )
       << transform.mapX( bbox.left ) << " "
       << transform.mapY( bbox.top - bbox.height ) << " "
       << transform.mapX( bbox.left + bbox.width ) << " "
       << transform.mapY( bbox.top ) << std::endl;
  file << "%Magnification: 1.0000" << std::endl;
  file << "%%EndComments" << std::endl;

  file << std::endl;
  file << "/cp {closepath} bind def" << std::endl;
  file << "/ef {eofill} bind def" << std::endl;
  file << "/gr {grestore} bind def" << std::endl;
  file << "/gs {gsave} bind def" << std::endl;
  file << "/sa {save} bind def" << std::endl;
  file << "/rs {restore} bind def" << std::endl;
  file << "/l {lineto} bind def" << std::endl;
  file << "/m {moveto} bind def" << std::endl;
  file << "/rm {rmoveto} bind def" << std::endl;
  file << "/n {newpath} bind def" << std::endl;
  file << "/s {stroke} bind def" << std::endl;
  file << "/sh {show} bind def" << std::endl;
  file << "/slc {setlinecap} bind def" << std::endl;
  file << "/slj {setlinejoin} bind def" << std::endl;
  file << "/slw {setlinewidth} bind def" << std::endl;
  file << "/srgb {setrgbcolor} bind def" << std::endl;
  file << "/rot {rotate} bind def" << std::endl;
  file << "/sc {scale} bind def" << std::endl;
  file << "/sd {setdash} bind def" << std::endl;
  file << "/ff {findfont} bind def" << std::endl;
  file << "/sf {setfont} bind def" << std::endl;
  file << "/scf {scalefont} bind def" << std::endl;
  file << "/sw {stringwidth} bind def" << std::endl;
  file << "/tr {translate} bind def" << std::endl;
  file << " 0.5 setlinewidth" << std::endl;

  file << "newpath " 
       << transform.mapX( bbox.left ) 
       << " " << transform.mapY( bbox.top ) << " m " 
       << transform.mapX( bbox.left )
       << " " << transform.mapY( bbox.top - bbox.height ) << " l " 
       << transform.mapX( bbox.left + bbox.width  ) << " "
       << transform.mapY( bbox.top - bbox.height ) << " l " 
       << transform.mapX( bbox.left + bbox.width ) << " "
       << transform.mapY( bbox.top ) << " l " 
       << " cp clip " << std::endl;
 
  // Draw the background color if needed.
  if ( _backgroundColor != Color::none ) { 
    Rectangle r( bbox, Color::none, _backgroundColor, 0.0f );
    r.flushPostscript( file, transform );
  }

  // Draw the shapes
  std::vector< Shape* > shapes = _shapes;

  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();

  while ( i != end ) {
    (*i)->flushPostscript( file, transform );
    ++i;
  }
  file << "showpage" << std::endl;
  file << "%%Trailer" << std::endl;
  file << "%EOF" << std::endl;
  file.close();
}

void
Board::saveFIG( const char * filename, float scale ) const
{
  std::ofstream file( filename );
  TransformFIG transform;
  Rect bbox = boundingBox();
  transform.setBoundingBox( bbox );
  transform.setDepthRange( _shapes );
  if ( scale != 1.0 ) transform.furtherScale( scale );

  file << "#FIG 3.2  Produced by the Board library (C)2007 Sebastien Fourey\n";
  file << "Portrait\n";
  file << "Center\n";
  file << "Metric\n";
  file << "A4\n";
  file << "100.00\n";
  file << "Single\n";
  file << "-2\n";
  file << "1200 2\n";

  std::map<Color,int> colormap;
  int maxColor = 32;


  colormap[Color(0,0,0)] = 0; 
  colormap[Color(0,0,255)] = 1; 
  colormap[Color(0,255,0)] = 2; 
  colormap[Color(0,255,255)] = 0; 
  colormap[Color(255,0,0)] = 4; 
  colormap[Color(255,0,255)] = 0; 
  colormap[Color(255,255,0)] = 6; 
  colormap[Color(255,255,255)] = 7;


  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  while ( i != end ) { 
    if ( colormap.find( (*i)->penColor() ) == colormap.end() 
	 && (*i)->penColor().valid() )
      colormap[ (*i)->penColor() ] = maxColor++;
    if ( colormap.find( (*i)->fillColor() ) == colormap.end()
	 && (*i)->fillColor().valid() )
      colormap[ (*i)->fillColor() ] = maxColor++;
    ++i;
  }

  if ( colormap.find( _backgroundColor ) == colormap.end()
       && _backgroundColor.valid() )
    colormap[ _backgroundColor ] = maxColor++;
  
  // Write the colormap
  std::map<Color,int>::const_iterator iColormap = colormap.begin();
  std::map<Color,int>::const_iterator endColormap = colormap.end();
  char colorString[255];
  while ( iColormap != endColormap ) {
    sprintf( colorString,"0 %d #%02x%02x%02x\n",
	     iColormap->second,
	     iColormap->first.red(),
	     iColormap->first.green(),
	     iColormap->first.blue() );
    if ( iColormap->second >= 32 ) file << colorString;
    ++iColormap;
  }

  // Draw the background color if needed.
  if ( _backgroundColor != Color::none ) { 
    Rectangle r( bbox, Color::none, _backgroundColor, 0.0f );
    r.depth( std::numeric_limits<int>::max() );
    r.flushFIG( file, transform, colormap );
  }

  // Draw the shapes.
  i = shapes.begin();
  while ( i != end ) {
    (*i)->flushFIG( file, transform, colormap );
    ++i;
  }  
  file.close();
}

void
Board::saveSVG( const char * filename, float scale ) const
{
  float ppmm = 720/254.0f;
  std::ofstream file( filename );
  TransformSVG transform;
  Rect bbox = boundingBox();
  transform.setBoundingBox( bbox );
  if ( scale != 1.0 ) transform.furtherScale( scale );

  file << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
  file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
  file << " \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
  file << "<svg width=\"210mm\" height=\"297mm\" " << std::endl;
  file << "     viewBox=\"0 0 " << 210*ppmm << " " << 297*ppmm << "\" " << std::endl;
  file << "     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" >" << std::endl;
  file << "<desc>" << filename << ", created with the Board library (C) 2007 Sebastien Fourey" << "</desc>" << std::endl;
  
  // Draw the background color if needed.
  if ( _backgroundColor != Color::none ) { 
    Rectangle r( bbox, Color::none, _backgroundColor, 0.0 );
    r.flushSVG( file, transform );
  }
  
  // Draw the shapes.
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  while ( i != end ) {
    (*i)->flushSVG( file, transform );
    ++i;
  }  
  file << "</svg>" << std::endl;
  file.close();
}

void
Board::save( const char * filename ) const 
{
  const char * extension = filename + strlen( filename );
  while ( extension > filename && *extension != '.' ) 
    --extension;
  if ( !(strcmp( extension, ".eps" )) || !(strcmp( extension, ".EPS" )) ) {
    saveEPS( filename );
    return;
  }
  if ( !(strcmp( extension, ".fig" )) || !(strcmp( extension, ".FIG" )) ) {
    saveFIG( filename );
    return;
  }
  if ( !(strcmp( extension, ".svg" )) || !(strcmp( extension, ".SVG" )) ) {
    saveSVG( filename );
    return;
  }
}


} // namespace BoardLib;

/**
 * @example examples/arithmetic.cc
 * @example examples/example1.cc
 * @example examples/example2.cc
 */

/**
 * @mainpage Board - A C++ library for simple Postscript, SVG, and XFig drawings.
 *
 * <img align=left src="http://www.greyc.ensicaen.fr/~seb/images/board.png"> (C) 2007 Sébastien Fourey - GREYC ENSICAEN 
 *
 * @section intro_sec Introduction
 *
 * The board library allows simple drawings in:
 * <ul>
 *  <li>Encapsulated Postcript files (EPS) ;
 *  <li>XFig files (FIG) ;
 *  <li>Scalable Vector Graphics files (SVG).
 * </ul>
 *
 * The main class of the library is the #BoardLib#Board class. It is intented to be as simple as possible
 * so that it can be used quickly in programs to generate the kind of figure one would rather
 * not draw by hand, but which can be easily drawn by a computer (C++) program.
 *
 * @section examples_sec Code examples
 *
 * <ul>
 * <li>Se the "Examples" tab above.
 * </ul>
 *
 * @section links_sec Links
 *
 * <ul>
 * <li>Visit the <a href="http://www.greyc.ensicaen.fr/~seb/board/">Board homepage</a>.</li>
 * </ul>
 */
