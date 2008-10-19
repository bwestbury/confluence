// 
//  crc.hpp
//  
//  Created by Sebastian Jörn on 2008-07-30.
//  Copyright 2008 Sebastian Jörn. All rights reserved.
// 

#ifndef CRC_HPP
#define CRC_HPP

#include <cmath>
#include "triang.hpp"
#include "BoardLib/Board.h"
using namespace BoardLib;

const long double pi = 3.14159265359;

const int number_of_iterations = 2000;//Number of iterations for the uniform neighbours approach.
const long double margin_of_error = 0.1;//The margin of error for the uniform neighbours approach.


class LComplex {//A labeled complex, which has a packing label if packing is set to true.
public:
	LComplex(): size(0), c(NULL), packing(false), label(NULL), neighbours(NULL), nsizes(NULL) {}
	LComplex(CombPoint *cp, int s, int **n, int *ns);
	LComplex(IGraph& igraph, int **n, int *ns);
	~LComplex();

	void layout();//Follows the meta-code desribed in Stephenson's book p. 30.
	void make_packing();//The key function of this class; makes the label of this complex into a packing label.
	bool ispacking();
	CombPoint &operator[](int index) { return c[index];}
	long double getradius(int i) { if(packing) return label[i]; else return 0;}
	long double getsize() {return size;}
	int getnum_barycentres() {int barycentres = 0; for(int i = 0; i < size; i++) if(c[i].type == c[i].Barycentre) barycentres++; return barycentres;};
	void info();
private:
	//auxiliary functions for layout:
	void place_circle(int v, int u, int w, long double (*&coordinates)[2], bool *&placed);
	void layout_draw(long double (*coordinates)[2], bool *placed);//Utilises Sebastien Fourey's BoardLib for drawing SVG-files.

	//auxiliary functions for make_packing:
	long double angle_sum(int circle);//Calculates the angle sum (with the current labels) at the CombPoint circle.
	long double target_sum(int circle);//Return the target angle sum depending on the type of CombPoint we're looking at. [N.B. This is NOT the same target angle sum that Stephenson talks about.]
	long double target_sum_BoundaryV();//Checks how many boundary vertices there are, and returns the target angle sum for such an n-gon.
	long double distance();//The complex's distance to a packing label (described in Stephenson's book p. 243).
	void uniformneighbour_model(int circle);//Performs the uniform neighbour model calculation.

	//DATA MEMBERS
	int size;//Number of CombPoints in the labelled complex.
	CombPoint *c;//The vertices, or centres of the circles.
	bool packing;//Whether or not the labelled complex is a packing.
	long double *label;//Putative radii.
	int **neighbours;//The lists of neighbours for each CombPoint.
	int *nsizes;//Size of each list (subarray) in neighbours.
};

long double angle(long double r, long double r1, long double r2);//Law of Cosines.

#endif /* CRC_HPP */
