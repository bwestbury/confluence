// 
//  Main.cpp
//  confluence
//  
//  Created by David Howden on 2007-06-20.
//  Copyright 2007 David Howden. All rights reserved.
// 

#include <iostream>
#include <fstream>

#include "Graph.hpp"

using namespace std;
using namespace Confluence;

int main (int argc, char const* argv[])
{
	Graph g;
	g = g.createPolygon(3);
	cout << g;
	
	//g.output();
	
	Graph g1(3), g2(3), g3(3);
	//g1.output();

	cout << "***** g1.glue(g2, 1)" << endl;
	g1.glue(g2, 1);
	//g1.output();
	
	cout << "***** g1.rotate(1)" << endl;
	g1.rotate(1);
	//g1.output();
	
	cout << "***** g1.glue(g3, 2)" << endl;
	g1.glue(g3, 2);
	g1.output();
	
	g1.normalForm();
	
	g1.output();
	
	if(g1.isomorphic(g)) {
		cout << "Woohooooooooooo!" << endl;
	} else {
		cout << "****!" << endl;
	}
	
	return 0;
}