// 
//  closure_test.cpp
//  confluence
//  
//  Created by David Howden on 2007-08-31.
//  Copyright 2007 David Howden. All rights reserved.
// 

#include "graph.hpp"
#include "SVGGraphDrawer.hpp"
#include <iostream>

using namespace Confluence;
using namespace std;

int main (int argc, char const *argv[])
{
   Graph g(3);
   //cout << "First Output:" << endl;
   //g.output();
   //g.closure();
   //cout << "Closure:" << endl;
   //g.output();
   
   SVGGraphDrawer gd;
   gd.constructGraphDiagram(g);
   gd.outputGraphDiagram("closure_test.svg");
   
   
   return 0;
}