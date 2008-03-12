// 
//  Test.cpp
//  confluence
//  
//  Created by David Howden on 2007-06-22.
//  Copyright 2007 David Howden. All rights reserved.
// 

#include "Confluence.hpp"
#include "SVGGraphDrawer.hpp"
#include "Graph.hpp"

using namespace Confluence;
using namespace std;

int main (int argc, char const* argv[])
{
	Graph g(10);
	
	cout << g << endl;
	
	//Graph<int> g("test.graph");
	//cout << g << endl;
	
	Graph g1, g2;
	g1 = g1.createPolygon(6);
	g2 = g2.createPolygon(6);
	
    SVGGraphDrawer gd;
    gd.constructGraphDiagram(g1);
    gd.outputGraphDiagram("g1.svg");
	
	// 
	// cout << g1 << endl;
	// cout << g2 << endl;
	// 
	// if(g1.isomorphic(g2)) {
	// 	cout << "Woohoooooooo!" << endl;
	// } else {
	// 	cout << "Boooooooo!" << endl;
	// }
	
	// LinComb<int,double> tester(12, 12.34), tester2(10, 10.001), tester3;
	// cout << tester << endl << tester2 << endl;
	// 
	// tester3 = tester + tester2;
	// cout << tester3 << endl;
	// 
	// tester*=2;
	// cout << tester << endl;
	// tester*=23.43;
	// cout << tester << endl;
	// tester3*=100;
	// cout << tester3 << endl;
	// 
	// cout << "Saving to file tester3.bin" << endl;
	// serialiseObjectBinary(tester3, "tester3.bin");
	// 
	// LinComb<int, double> tester4(1,1.0);
	// //unserialiseObjectBinary(tester4, "tester3.bin");
	// cout << tester4 << endl;
	// 
	// Tester t(1,2,3);
	// t.output();
	// Tester t2(0,0,0);
	// t2.output();
	// 
	// serialiseObjectBinary(t, "t.bin");
	// unserialiseObjectBinary(t2, "t.bin");
	// t2.output();
	
	
	return 0;
}