// 
//  GraphDrawer.hpp
//  confluence
//  
//  Created by David Howden on 2007-08-21.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _GRAPHDRAWER_HPP_
#define _GRAPHDRAWER_HPP_

#include <map>
#include <vector>
#include <utility>
#include <iterator>

#include "Graph.hpp"

using namespace std;

namespace Confluence {

class GraphDrawer
{
public:
    GraphDrawer () {}
    ~GraphDrawer () {}
    
    void constructGraphDiagram (Graph graph)
    {
        g = graph;
        cout << "Computing closure...";
        g.closure();
        cout << " Done." << endl;
        
        cout << "Graph:" << g << endl;
        
        vx.clear();
        
        cout << "Calling numberVertices(1)..." << endl;
        numberVertices(1);
        //cout << "vx:" << endl;
        
        for(int j = 0; j < g.N; j++) {
           cout << "vx(" << j << ") = " << vx[j] << endl;
        }
        
        cout << "Number of vertices: " << n << endl;
        v = n;
        
        cout << " Done." << endl;
        
        cout << "Calling labelX()...";
        labelX();
        
        cout << " Done." << endl;
        
        for(int j = 1; j <= v; j++) {
           cout << "X(" << j << ") = " << X[j] << endl;
        }
        
        cout << "Calling labelY()...";
        labelY();
        cout << " Done." << endl;
        
        for(int j = 1; j <= v; j++) {
           cout << "Y(" << j << ") = " << Y[j] << endl;
        }
        
        int a,b;
        
        x1.clear();
        x2.clear();
        y1.clear();
        y2.clear();
        
        for(int i = 0; i < g.N; i++) {
            a = vx[i];
            b = vx[g.e[i]];
            if (a < b) {
                x1.push_back(X[a]);
                y1.push_back(Y[a]);
                x2.push_back(X[b]);
                y2.push_back(Y[b]);
            }
        }
    }
    
    void outputGraphDiagram (char *filename)
    {
        cout << "You need to override GraphDrawer::outputGraphDiagram(char* filename)" << endl;
    }

protected:
    
    vector<int> x1, x2, y1, y2;

private:
	void numberVertices (int a) 
	{   
        count = 0;
        n = 1;
        
        for (int i = 0; i < g.N; i++) {
            vx[i] = -1;
        }
        
        labelNumberVertices(a, g.N);
        numberNumberVertices(0);
        labelNumberVertices(a,n);
	}
	
	void labelNumberVertices (int k, int m)
	{
      int first = k;
	    do {
            vx[k] = m;
            count++;
            k=g.c[k];
        } while (k != first);
	}
	
	void numberNumberVertices (int k) {
        if (vx[k] == -1) {
            labelNumberVertices(k, n);
            n++;
            if(count < g.N) {
                numberNumberVertices(g.c[g.e[k]]);
            } else {
                return;
            }
        } else {
            numberNumberVertices(g.e[g.c[k]]);
        }
	}
	    
    void labelX () {
        n = 0;
        X.clear();
        labelCoord(X, g.c, 0);
    }
    
    void labelY ()
    {
        map<int, int> c_inv;
        for (map<int,int>::iterator iter = g.c.begin(); iter != g.c.end(); iter++) {
            c_inv[iter->second] = iter->first;
        }
        n = 0;
        Y.clear();
        labelCoord(Y, c_inv, 0);
    }
    
    void labelCoord (map<int, int> &coord, map<int,int> &map, int k)
    {      
        int a, b, r, t;
        
        a = vx[k];
        coord[a] = n;
        n++;
        if (n < v) {
           if(k == 0) {
              r = 1;
           } else {
              r = k;
              while (vx[g.e[r]] > a || vx[g.e[map[r]]] < a) {
                r = map[r];
                cout << "r = " << r << " ";
             }
             r = map[r]; 
           }
            //cout << "r"
            do {
               cout << "*";
               t = g.e[r];
               b = vx[t];
               if (vx[r] > b && vx[g.e[map[r]]] < b) {
                  cout << "Woohoooooo!" << endl;
                  labelCoord(coord, map, t);
               }
               r = map[r];
               cout << " new r =" << r << endl;
               cout << " vx(r) =" << vx[r] << endl;
               cout << " vx[g.e[r]] =" << vx[g.e[r]] << endl;
            } while (vx[r] < vx[g.e[r]]);     
        }
    }
    
    Graph g;
        
    int count;
    int n;
    int v;
    map<int, int> vx, X, Y;
};

}

#endif /* _GRAPHDRAWER_HPP_ */
