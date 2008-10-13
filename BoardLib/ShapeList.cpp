/* -*- mode: c++ -*- */
/**
 * @file   ShapeList.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  Definition of the ShapeList and Group classes.
 * 
 * @copyright
 */
#include "ShapeList.h"

namespace BoardLib {

  //
  // Definition of the ShapeList methods.
  //

ShapeList::~ShapeList()
{
  free();
}

void
ShapeList::clear()
{
  free();
  _shapes.clear();
  _nextDepth = std::numeric_limits<int>::max() - 1;
}

void
ShapeList::free()
{
  std::vector<Shape*>::const_iterator i = _shapes.begin();
  std::vector<Shape*>::const_iterator end = _shapes.end();
  while ( i != end ) {
    delete *i;
    ++i;
  }  
}

ShapeList::ShapeList( const ShapeList & other ) : Shape( other )
{
  if ( ! other._shapes.size() ) return;  
  _shapes.resize( other._shapes.size(), 0 );
  std::vector<Shape*>::iterator t = _shapes.begin();
  std::vector<Shape*>::const_iterator i = other._shapes.begin();
  std::vector<Shape*>::const_iterator end = other._shapes.end();
  while ( i != end ) {
    *t = (*i)->clone();
    ++i; ++t;
  }
}

ShapeList &
ShapeList::operator=( const ShapeList & other )
{
  free();
  if ( ! other._shapes.size() ) return *this;
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

ShapeList &
ShapeList::operator<<( const Shape & shape )
{
  if ( typeid( shape ) == typeid( ShapeList ) ) {
    // Flat insertion, respecting the same depth order.
    const ShapeList & sl = dynamic_cast<const ShapeList &>( shape );
    std::vector<Shape*> shapes = sl._shapes;
    stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
    std::vector<Shape*>::iterator i = shapes.begin();
    std::vector<Shape*>::iterator end = shapes.end();
    while ( i != end ) {
      Shape * s = (*i)->clone();
      s->depth( _nextDepth-- );
      _shapes.push_back( s );
      ++i;
    }
  } else {
    Shape * s = shape.clone();
    if ( s->depth() == -1 ) s->depth( _nextDepth-- );
    _shapes.push_back( s );
  }
  return *this;
}

Point
ShapeList::center() const {
  std::vector<Shape*>::const_iterator i = _shapes.begin();
  std::vector<Shape*>::const_iterator end = _shapes.end();
  float f = 1.0 / _shapes.size();
  Point r(0,0);
  while ( i != end ) {
    r += f * (*i)->center();
    ++i;
  }
  return r;
}

Shape &
ShapeList::rotate( double angle, const Point & center )
{
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    (*i)->rotate( angle, center );
    ++i;
  }
  return *this;
}

Shape &
ShapeList::rotate( double angle )
{
  return rotate( angle, center() );
}

Shape &
ShapeList::translate( double dx, double dy )
{
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    (*i)->translate( dx, dy );
    ++i;
  }
  return *this;  
}

Shape &
ShapeList::scale( double sx, double sy )
{
  Point c = center();
  Point delta;
  std::vector<Shape*>::iterator i = _shapes.begin();
  std::vector<Shape*>::iterator end = _shapes.end();
  while ( i != end ) {
    delta = (*i)->center() - c;
    delta.x *= sx;
    delta.y *= sy;
    (*i)->scale( sx, sy );
    delta = ( c + delta ) - (*i)->center();
    (*i)->translate( delta.x, delta.y );
    ++i;
  }
  return *this;
}

void
ShapeList::flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  stream << "%%% Begin ShapeList\n";
  while ( i != end ) {
    (*i)->flushPostscript( stream, transform );
    ++i;
  }
  stream << "%%% End ShapeList\n";
}
  
void
ShapeList::flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  while ( i != end ) {
    (*i)->flushFIG( stream, transform, colormap );
    ++i;
  }  
}

void
ShapeList::flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const
{
  std::vector< Shape* > shapes = _shapes;
  stable_sort( shapes.begin(), shapes.end(), shapeGreaterDepth );
  std::vector< Shape* >::const_iterator i = shapes.begin();
  std::vector< Shape* >::const_iterator end = shapes.end();
  stream << "<g>\n";
  while ( i != end ) {
    (*i)->flushSVG( stream, transform );
    ++i;
  }  
  stream << "</g>\n";
}

Rect
ShapeList::boundingBox() const
{
  Rect r;
  std::vector< Shape* >::const_iterator i = _shapes.begin();
  std::vector< Shape* >::const_iterator end = _shapes.end();
  if ( i == end ) return r;
  r = (*i)->boundingBox();
  ++i;
  while ( i != end ) { 
    r = r || (*i)->boundingBox();
    ++i;
  }
  return r;
}

Shape *
ShapeList::clone() const
{
  ShapeList * g = new ShapeList( *this );
  return g;
}

  //
  // Definition of the Group methods.
  //

void
Group::flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const
{
  stream << "%%% Begin Group\n";
  ShapeList::flushPostscript( stream, transform );
  stream << "%%% End Group\n";
}
  
void
Group::flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const
{
  Rect bbox = boundingBox();
  stream << "# Begin group\n";
  stream << "6 "
	 << transform.mapX( bbox.left ) << " "
	 << transform.mapY( bbox.top ) << " "
	 << transform.mapX( bbox.left + bbox.width ) << " "
	 << transform.mapY( bbox.top - bbox.height ) << "\n";
  ShapeList::flushFIG( stream, transform, colormap );
  stream << "-6\n";
  stream << "# End Group\n";
}

void
Group::flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const
{
  stream << "<g>\n";
  ShapeList::flushSVG( stream, transform );
  stream << "</g>\n";
}

Shape *
Group::clone() const
{
  Group * g = new Group( *this );
  return g;
}

Group &
Group::operator=( const Group & other )
{
  ShapeList::operator=( other );
  return *this;
}

Group &
Group::operator<<( const Shape & shape )
{
  ShapeList::operator<<( shape );
  return *this;
}

} // namespace BoardLib
