// 
//  SVGTest.cpp
//  confluence
//  
//  Created by David Howden on 2007-06-29.
//  Copyright 2007 David Howden. All rights reserved.
// 

#include "SVGLineDiagram.hpp"

int main (int argc, char const* argv[])
{
	SVGLineDiagram d(12, 12, 1200, 1200);
	for(int i = 0; i < 100; i++) {
		for(int j = 0; j < i; j++) {
			d.addLine(100+i, 100+j, i+j, i+j);
		}
	}
	d.saveToFile("SVGTest.svg");
	return 0;
}