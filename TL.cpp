// 
//  TL.cpp
//  confluence
//  
//  Temperley-Lieb Testing using Confluence functions
//  
//  Created by David Howden on 2007-06-27.
//  Copyright 2007 David Howden. All rights reserved.
// 

// #include "Confluence.hpp"
#include "Graph.hpp"
#include "SVGGraphDrawer.hpp"

#include <map>
#include <set>
#include <iostream>
//#include <vector>
#include <list>

using namespace std;
using namespace Confluence;

int main (int argc, char const* argv[])
{
	set<int> I;
	I.insert(0);
	I.insert(1);
	map<int, int> c, e;
	c[0] = 1;
	c[1] = 0;
	e[0] = -1;
	e[1] = -1;
	Graph cup(2, 0, I, c, e);
	cout << cup << endl;
	
	Graph temp;
	temp = cup;
	temp.rotate(1);
	cout << temp << endl;
	
	
	int n;
	cout << "Please enter a value for n: ";
	cin >> n;
	
	map<int, list < Graph > > C;

	list< Graph > C_0;
	Graph empty(0,0);
	C_0.push_back(empty);
	C[0] = C_0;
	
	list< Graph >::iterator iter, iter2;
	Graph g1, g2, g;
	list< Graph > tempList;
	cout << "************* STARTING CATALAN LOOP *******************" << endl;
	for(int j = 1; j < n; j++) {
		for(int i = 0; i < j; i++) {
			for(iter = C[i].begin(); iter != C[i].end(); iter++) {
				g1 = (*iter);
				for(iter2 = C[j-i-1].begin(); iter2 != C[j-i-1].end(); iter2++) {
					g = cup;
					g2 = (*iter2);
					g.glue(g1, 0);
					//cout << "C[" << j << "] ***** AFTER GLUE CUP *****" << endl;
					//g.output();
					g.rotate(1);
					//cout << "C[" << j << "] ***** AFTER ROTATE   *****" << endl;
					//g.output();
					
					cout << "C[" << j << "] ***** g2 *****" << endl;
					//g2.output();
					g.glue(g2, 0);
					//cout << "C[" << j << "] ***** AFTER GLUE g2 *****" << endl;
					g.output();
					tempList.push_back(g);
					
					
				}
			}
		}
		C[j] = tempList;
		cout << "************** END OF ONE LIST *************" << endl;
		tempList.clear();
	}
	cout << "************* ENDING CATALAN LOOP ******************" << endl;
	
	
	// for(int i = 0; i < n; i++) {
	// 	tempList = C[i];
	// 	cout << "****************" << endl << " i = " << i << " (" << tempList.size() << ")" << endl << "****************" << endl;
	// 	for(iter = tempList.begin(); iter != tempList.end(); ++iter) {
	// 		iter->checkMaps();
	// 		//iter->output();
	// 	}
	// }
	
	// assume that n > 5
	
	cout << "BEGIN" << endl;
	
	cout << C[3].size() << endl;
	
	iter = C[3].begin();
	g1 = (*iter);
	g1.output();

	iter = C[3].end();
	iter--;
	//iter->output();
	//return;
	g2 = (*iter);
	//return;
	g2.output();
	
	//return;
	
	Graph t = g1;
	
	t.glue(g2, 3);
	cout << "After Glued:" << endl;
	t.output();
	t.normalForm();
	cout << "After normalForm():" << endl;
	t.output();
	t.removeLoops();
	cout << "After Removed Loops:" << endl;
	t.output();
	
    SVGGraphDrawer d;
    cout << "Here";
    d.constructGraphDiagram(t);
    cout << "here2";
    d.outputGraphDiagram("t.svg");
    cout << "here3";
		
	// if(t.isomorphic(g1)) {
	// 	cout << "Woohoooo!" << endl;
	// } else {
	// 	cout << "Boooooo!" << endl;
	// }
	
	int count = 0;
	cout << "**************************************************************" << endl;
	for(iter = C[3].begin(); iter != C[3].end(); iter++) {
		
		if((*iter).isomorphic(t)) {
			iter->output();
			count++;
		}
	}
	
	cout << endl << endl << count << endl << endl;

	return 0;
}