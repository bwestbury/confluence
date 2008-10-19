/* -*- mode: c++ -*- */
/**
 * @file   Rect.h
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
#ifndef _BOARD_RECT_H_
#define _BOARD_RECT_H_

#include <iostream>

namespace BoardLib {

/**
 * The Rectangle structure.
 * @brief Struct representing a rectangle on the plane. 
 */
struct Rect {
  float left;			/**< Coordinate of the left side. */
  float top;			/**< Coordinate of the upper side. */
  float width;			/**< Width of the rectangle. */
  float height;		/**< Height of the rectangle. */
  
  /** 
   * Rect constructor.
   * 
   * @param left 
   * @param top 
   * @param width 
   * @param height 
   * 
   * @return 
   */
  Rect( float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f )
    :left( left ), top( top ), width( width ), height( height ) { } 
};

/** 
 * Computes the bounding box of two bounding boxes.
 * 
 * @param rectA A first rectangle.
 * @param rectB A second rectangle.
 * 
 * @return The smallest rectangle that contains both rectA and rectB.
 */
Rect operator||( const Rect & rectA, const Rect & rectB );


} // mamespace BoardLib

/** 
 * Stream output operator for Rect structure.
 * 
 * @param out An output stream.
 * @param rect A Rect struct.
 * 
 * @return The output stream.
 */
std::ostream & operator<<( std::ostream & out, const BoardLib::Rect & rect );

#endif // _RECT_H_
