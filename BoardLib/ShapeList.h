/* -*- mode: c++ -*- */
/**
 * @file   ShapeList.h
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  Classes ShapeList and ShapeGroup
 *
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
#ifndef _BOARD_SHAPELIST_H_
#define _BOARD_SHAPELIST_H_

#include "Shapes.h"
#include <algorithm>//[Added by Sebastian Jörn]

namespace BoardLib {

/**
 * The ShapeList structure.
 * @brief A group of shapes
 */
struct ShapeList : public Shape {
  
  ShapeList( int depth = -1 )
    : Shape( Color::none, Color::none, 1.0, ButtCap, MiterJoin, depth ),
      _nextDepth( std::numeric_limits<int>::max() - 1 )
  { }

  ShapeList( const ShapeList & other );

  ~ShapeList();
  
  void clear();

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

  Shape * clone() const;
  
  ShapeList & operator=( const ShapeList & other );

  ShapeList & operator<<( const Shape & shape ); 
  
protected:
  std::vector<Shape*> _shapes;	/**< The vector of shapes. */
  int _nextDepth;		/**< The depth of the next figure to be added.  */

  /** 
   * Free the memory used by the shapes in the shape vector.
   */
  void free();
};

/**
 * The Group structure.
 * @brief A group of shapes. A group is basically a ShapeList except that
 * when rendered in eithe an SVG of a FIG file is a true compound element.
 */
struct Group : public ShapeList {
  
  Group( int depth = -1 ):ShapeList( depth ) { }
  
  Group( const Group & other ) : ShapeList( other ) { };

  ~Group() { };

  void flushPostscript( std::ostream & stream,
			const TransformEPS & transform ) const;
  
  void flushFIG( std::ostream & stream,
		 const TransformFIG & transform,
		 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
		 const TransformSVG & transform ) const;

  Group & operator=( const Group & other );

  Group & operator<<( const Shape & shape ); 

  Shape * clone() const;
};


} // namespace BoardLib 

#endif /* _SHAPELIST_H_ */

