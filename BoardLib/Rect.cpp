/* -*- mode: c++ -*- */
/**
 * @file   Rect.cpp
 * @author Sebastien Fourey <http://www.greyc.ensicaen.fr/~seb>
 * @date   Sat Aug 18 2007
 * 
 * @brief  
 * @copyright
 */
#include "Rect.h"

namespace BoardLib {

Rect
operator||( const Rect & rectA, const Rect & rectB )
{
  Rect rect;
  rect.top = ( rectA.top > rectB.top ) ? rectA.top : rectB.top;
  rect.left = (rectA.left < rectB.left) ? rectA.left : rectB.left;
  if ( rectA.left + rectA.width > rectB.left + rectB.width )
    rect.width = rectA.left + rectA.width - rect.left;
  else
    rect.width = rectB.left + rectB.width - rect.left;
  if ( rectA.top - rectA.height < rectB.top - rectB.height )
    rect.height = rect.top - ( rectA.top - rectA.height );
  else
    rect.height = rect.top - ( rectB.top - rectB.height );
  return rect;
}

} // namespace BoardLib

std::ostream &
operator<<( std::ostream & out, const BoardLib::Rect & rect )
{
  out << "Rect(" 
      << rect.left << "," << rect.top
      << "+" << rect.width << "x" << rect.height << ")";
  return out;
}
