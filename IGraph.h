// 
//  LComplex.hpp
//  
//  Created by Sebastian Jörn on 2008-07-30.
//  Copyright 2008 Sebastian Jörn. All rights reserved.
// 

#ifndef IGRAPH_H
#define IGRAPH_H

#include "Graph.hpp"

using namespace Confluence;

struct CombPoint {//All types of (combinatorial) points in the barycentric subdivision.
	//Look over these to see if any are redundant...
	enum CPointType {
		None,
		InteriorV,
		GBoundaryV,
		BoundaryV,
		InteriorE,
		BoundaryE,
		Barycentre
	};
	CombPoint(CPointType t): type(t) {}
	CombPoint(): type(None) {}
	CPointType type;//type of vertex/edge or barycentre. (more importantly, tells whether edge or vertex or barycentre).
};

struct IGCombPoint: public CombPoint {
	IGCombPoint(): orbit(-1), CombPoint() {};
	IGCombPoint(int o, CPointType t): orbit(o), CombPoint(t) {};
	int orbit;//least point in orbit representing vertex/edge. [-1 for barycentres or none-points]
};

class IGraph {//An indexed Graph, with additional info distinguishing points in the original Graph and points in gnew.
public:
	friend class LComplex;
	IGraph(): gp(NULL), newdef(-1), size(0), faces(NULL), face_sizes(NULL) {}
	IGraph(const IGraph &);
	//The following constructor will index all CombPoints in the Graph g, including barycentres.
	//Input is ready-made planar graph (including the boundary), and an integer giving a lower bound for new-defined vertices (for the boundary).
	IGraph(Graph &gnew, int ndef);
	~IGraph();

	static Graph configure(Graph &graph, int *boundary);//Takes graph and an array specifying where the boundary is.
	void getallneighbours(int **&neighbours, int *&neighboursize);//In counterclockwise order. Might wanna make this static.
	IGCombPoint &operator[](int index) { return gp[index];}
	int getsize() {return size;}
	int getfacecount();
	void info();
private:
	//auxiliary functions for constructor:
	IGCombPoint::CPointType checkCPtype(int orbit, bool vertex);//For a given orbitpoint, checks which type of vertex/edge it is.
	void addCP(int orbit, IGCombPoint::CPointType);
	void checkorbit(map<int, int> &d, bool *indexed, int orbitstart);//Checks the orbit of map d starting at orbitstart (and ticks them off the list).
	void indexbarycentres();

	//auxiliary functions for indexbarycentres:
	int findface(int *&face, int start, bool *inface);
	int findorbitstart(map<int, int> &d, int point);//Finds the least value in the orbit of map d containing the value point.
	void addface(int *newface, int newfacesize, int &facecount);
	void addtoface(int *&face, int &facesize, int orbitstart, bool vertex);

	//auxiliary functions for getallneighbours:
	void findneighbours(int cp, int **&neighbours, int *&neighboursizes);//Finds neighbours for a particular IGCombPoint (excluding barycentres).
	void addtotriplen(int x, int y, int z, int **&triplen, int &triplensize);//Adds triple of the form {successor, barycentre, predecessor}.

	//DATA MEMBERS
	Graph g;
	int newdef;//Gives additional info enabling distinction between original Graph and gnew. (Basically N of old Graph g).

	//All indexed points (i.e. vertices, edges and barycentres):
	IGCombPoint *gp;
	int size;
	//Data concerning barycentres:[the size of faces (and face_sizes) given by getfacecount()]
	int **faces;//Orbits in counterclockwise order.
	int*face_sizes;
};

//}
#endif /* IGRAPH_H */
