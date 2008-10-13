/* -*- mode: c++ -*- */
/**
 * @file   Transforms.h
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
#ifndef _BOARD_TRANSFORMS_H_
#define _BOARD_TRANSFORMS_H_

#include <limits>
#include <vector>

namespace BoardLib {

struct Rect;
struct Shape;

/**
 * The base class for transforms.
 * @brief 
 */
struct Transform {
public:
  Transform():_scale(1.0), _deltaX(0.0), _deltaY(0.0) { }
  virtual ~Transform() { };
  virtual float mapX( float x ) const;
  virtual float mapY( float y ) const = 0;
  virtual void apply( float & x, float & y ) const;
  virtual float scale( float x ) const;
  virtual void furtherScale( float x );
  virtual float rounded( float x ) const;
  virtual void setBoundingBox( const Rect & rect ) = 0;
protected:
  float _scale;
  float _deltaX;
  float _deltaY;
};

/**
 * The TransformEPS structure.
 * @brief Structure representing a scaling and translation
 * suitable for an EPS output.
 */
struct TransformEPS : public Transform {
public:
  float mapY( float y ) const;
  void setBoundingBox( const Rect & rect );
};

/**
 * The TransformFIG structure.
 * @brief Structure representing a scaling and translation
 * suitable for an XFig output.
 */
struct TransformFIG : public Transform {
public:
  TransformFIG():maxDepth(std::numeric_limits<int>::max()),minDepth(0) { }
  float rounded( float x ) const;
  float mapY( float y ) const;
  int mapWidth( float width ) const; 
  void setBoundingBox( const Rect & rect );
  void setDepthRange( const std::vector<Shape*> & shapes );
  int mapDepth( int depth ) const;
private:
  int maxDepth;
  int minDepth;
};

/**
 * The TransformSVG structure.
 * @brief Structure representing a scaling and translation
 * suitable for an SVG output.
 */
struct TransformSVG : public Transform {
public:
  float rounded( float x ) const;
  float mapY( float y ) const;
  float mapWidth( float width ) const; 
  void setBoundingBox( const Rect & rect );
};


} // namespace BoardLib 

#endif /* _TRANSFORMS_H_ */
