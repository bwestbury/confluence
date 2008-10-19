// 
//  triang.cpp
//  
//  Created by Sebastian Jörn on 2008-07-30.
//  Copyright 2008 Sebastian Jörn. All rights reserved.
// 

#include "triang.hpp"


IGraph::IGraph(Graph &gnew, int ndef): g(gnew), newdef(ndef)
{
	if(g.N > 0)
	{
		//---VERTICES---
		//Create our checklist:
		bool *indexed = new bool[g.N];
		for(int i = 0; i < g.N; i++)
			indexed[i] = false;
		//Index first vertex:
		gp = new IGCombPoint(0, CombPoint::InteriorV);
		size = 1;
		checkorbit(g.c, indexed, 0);
		//Index remaining vertices:
		for(int i = 0; i < g.N; i++)
		{
			if(!indexed[i])
			{
				CombPoint::CPointType t = checkCPtype(i, true);
				addCP(i, t);
				checkorbit(g.c, indexed, i);
			}
		}

		//---EDGES---
		//Reset our checklist for the indexing of edges:
		for(int i = 0; i < g.N; i++)
			indexed[i] = false;
		//Index all edges:
		for(int i = 0; i < g.N; i++)
		{
			if(!indexed[i])
			{
				CombPoint::CPointType t = checkCPtype(i, false);
				addCP(i, t);
				checkorbit(g.e, indexed, i);
			}
		}

		//---BARYCENTRES---
		indexbarycentres();
	}
	else
	{
		gp = NULL;
		size = 0;
	}
}

IGraph::IGraph(const IGraph &ig): g(ig.g), newdef(ig.newdef), size(ig.size)
{
	gp = new IGCombPoint[size];
	face_sizes = new int[size];
	faces = new int*[size];
	for(int i = 0; i < size; i++)
	{
		gp[i] = ig.gp[i];
		face_sizes[i] = ig.face_sizes[i];
		
		faces[i] = new int[face_sizes[i]];
		for(int j = 0; j < face_sizes[i]; j++)
			faces[i][j] = ig.faces[i][j];
	}
}

IGraph::~IGraph()
{
	int fc = getfacecount();
	for(int i = 0; i < fc; i++)
		delete [] faces[i];
	delete [] faces;
	delete [] face_sizes;
	delete [] gp;
}

void IGraph::addCP(int orbit, IGCombPoint::CPointType t)
{
	//Re-size gp:
	IGCombPoint *temp = gp;
	gp = new IGCombPoint[size+1];
	for(int j = 0; j < size; j++)
		gp[j] = temp[j];
	delete [] temp;

	//Add new point:
	gp[size] = IGCombPoint(orbit, t);
	size++;
}

void IGraph::addface(int *newface, int newfacesize, int &facecount)
{
	//Resizing faces:
	int **temp1 = faces;
	faces = new int*[facecount+1];
	for(int i = 0; i < facecount; i++)
		faces[i] = temp1[i];
	delete temp1;

	//Adding new face:
	faces[facecount] = newface;

	//Resizing face_sizes:
	int *temp2 = face_sizes;
	face_sizes = new int[facecount+1];
	for(int i = 0; i < facecount; i++)
		face_sizes[i] = temp2[i];
	delete temp2;
	face_sizes[facecount] = newfacesize;

	facecount++;
}

void IGraph::addtoface(int *&face, int &facesize, int orbitstart, bool vertex)
{
	//Resize face:
	int *temp = face;
	face = new int[facesize+1];
	for(int i = 0; i < facesize; i++)
		face[i] = temp[i];
	delete temp;

	//Adding next vertex/edge:
	if(vertex)
	{
		//Determine what the index is of our particular vertex:
		for(int i = 0; i < size; i++)
		{
			//The second condition here making sure that the CombPoint is indeed a vertex:
			if(gp[i].orbit == orbitstart && (gp[i].type == IGCombPoint::InteriorV || gp[i].type == IGCombPoint::GBoundaryV || gp[i].type == IGCombPoint::BoundaryV))
			{
				face[facesize] = i;
				break;
			}
		}
	}
	else//edge
	{
		//Determine what the index is of our particular edge:
		for(int i = 0; i < size; i++)
		{
			//The second condition here making sure that the CombPoint is indeed an edge:
			if(gp[i].orbit == orbitstart && (gp[i].type == IGCombPoint::InteriorE || gp[i].type == IGCombPoint::BoundaryE))
			{
				face[facesize] = i;
				break;
			}
		}
	}

	facesize++;
}

void IGraph::addtotriplen(int x, int y, int z, int **&triplen, int &triplensize)
{
	int **temp = triplen;
	triplen = new int*[triplensize+1];
	for(int i = 0; i < triplensize; i++)
		triplen[i] = temp[i];
	triplen[triplensize] = new int[3];
	triplen[triplensize][0] = x;
	triplen[triplensize][1] = y;
	triplen[triplensize][2] = z;
	triplensize++;
	delete [] temp;
}

IGCombPoint::CPointType IGraph::checkCPtype(int point, bool vertex)
{
	//point will always be least point in orbit, as we are going over all orbit values in ascending order. Thus there is no need to call the function findorbitstart.
	CombPoint::CPointType t;
	if(vertex)
	{
		//Set point to CombPoint is GBoundaryV.
		int orbitstart = point;
		do {
			if(g.e[point] < newdef)
				break;
			point = g.c[point];
		} while(point != orbitstart);

		//Determine what type of vertex we are looking at:
		if(point < newdef)
			t = CombPoint::InteriorV;
		else if(g.e[point] < newdef)
			t = CombPoint::GBoundaryV;
		else
			t = CombPoint::BoundaryV;
	}
	else//edge
	{
		//Determine what type of edge we are looking at:
		if(point < newdef)
			t = CombPoint::InteriorE;
		else
			t = CombPoint::BoundaryE;
	}
	return t;
}

void IGraph::checkorbit(map<int, int> &d, bool *indexed, int orbitstart)
{
	//Can assume orbitstart is already least point in orbit, since our indexing increases from 0.
	int point = orbitstart;
	indexed[point] = true;
	do {
		point = d[point];
		indexed[point] = true;
	} while(point != orbitstart);
}

Graph IGraph::configure(Graph &graph, int *boundary)
{
	Graph gnew;
	gnew.N = graph.N;//Will be incremented as we go along.
	gnew.c = graph.c;
	gnew.e = graph.e;

	//New InteriorE (going to first GBoundaryV)
	gnew.e[0] = gnew.N;
	gnew.e[gnew.N] = 0;
	gnew.N++;
	//First GBoundaryV:
	for(int i = 0; i < 2; i++)
		gnew.c[graph.N + i] = graph.N + 1 + i;
	gnew.N++;
	gnew.c[gnew.N] = graph.N;

	//Continue with all the BoundaryE:s and BoundaryV:s
	for(int i = 0; i < graph.B; i++)
	{
		//Add BoundaryE:s and BoundaryV:s before next GBoundaryV:
		for(int j = 0; j < boundary[i]; j++)
		{
			//BoundaryE
			gnew.e[gnew.N] = gnew.N + 1;
			gnew.e[gnew.N + 1] = gnew.N;
			gnew.N++;
			//BoundaryV
			gnew.c[gnew.N] = gnew.N + 1;
			gnew.c[gnew.N+1] = gnew.N;
			gnew.N++;
		}
		if(i == graph.B-1)
			break;//If we have reached last GBoundaryV, we are ready to tie the bag together.
		//Connect to next GBoundaryV:
		gnew.e[gnew.N] = gnew.N + 1;
		gnew.e[gnew.N+1] = gnew.N;
		gnew.N++;
		gnew.c[gnew.N] = gnew.N + 1;
		gnew.N++;
	}

	//---Tie the bag together---
	//Connect with first GBoundaryV:
	gnew.e[gnew.N] = graph.N + 1;
	gnew.e[graph.N+1] = gnew.N;
	gnew.N++;

	//Now go back and finish off the GBoundaryV:s
	int auxcounter = graph.N + 2;//The value in the orbit of the first GBoundary and the orbit of the first BoundaryE.
	for(int i = 0; i < graph.B-1; i++)
	{
		auxcounter += 2*boundary[i]+2;
		//Complete the orbit of the GBoundaryV:
		gnew.c[auxcounter] = gnew.N;
		gnew.c[gnew.N] = auxcounter-1;
		gnew.e[gnew.N] = i+1;
		//Complete the orbit of the InteriorE going to this GBoundaryV:
		gnew.e[i+1] = gnew.N;
		gnew.N++;
	}

	//Set the new values of B & E:
	gnew.B = 0;
	gnew.E = gnew.N;

	return gnew;
}

int IGraph::findface(int *&face, int start, bool *inface)
{
	face = NULL;
	int facesize = 0;
	int point = start;
	int orbitstart;//The least value in an orbit; this variable is used to temporarily store that value for a particular orbit.

	do {
		//Tick off the value of point from our checklist:
		inface[point] = true;

		//Apply c (adding the vertex in the process):
		orbitstart = findorbitstart(g.c, point);
		addtoface(face, facesize, orbitstart, true);//adding vertex
		point = g.c[point];

		//Apply e (adding the edge in the process):
		orbitstart = findorbitstart(g.e, point);
		addtoface(face, facesize, orbitstart, false);//adding edge
		point = g.e[point];
	} while(point != start);

	return facesize;
}

int IGraph::findorbitstart(map<int, int> &d, int point)
{
	int start = point;
	do {
		point = d[point];
		if(point < start)
			start = point;
	} while(point != start);
	return start;
}

void IGraph::findneighbours(int cp, int **&neighbours, int *&neighboursizes)
{
	int fc = getfacecount();
	int **triplen = NULL;
	int triplensize = 0;
	//Finding the triples:
	for(int i = 0; i < fc; i++)
	{
		for(int j = 0; j < face_sizes[i]; j++)
		{
			if(faces[i][j] == cp)
			{
				if(j == 0)
					addtotriplen(faces[i][j+1], i+size-fc, faces[i][face_sizes[i]-1], triplen, triplensize);
				else if(j == face_sizes[i]-1)
					addtotriplen(faces[i][0], i+size-fc, faces[i][j-1], triplen, triplensize);
				else
					addtotriplen(faces[i][j+1], i+size-fc, faces[i][j-1], triplen, triplensize);
				break;
			}
		}
	}

	//Finding the order of the triples (connecting them):
	int *order = new int[triplensize];
	for(int i = 0; i < triplensize; i++)
		order[i] = -1;
	order[0] = 0;
	int current;
	for(int i = current = 0; i < triplensize-1; i++)//-1 since first triple in order is set; looking for next triple. If BoundaryV or BoundaryE, this process will thus be skipped, since only have one pair of triples.
	{
		int temp = current;
		for(int j = 0; j < triplensize; j++)
		{
			if(current != j && triplen[current][2] == triplen[j][0])
			{
				order[i+1] = j;
				current = j;
				break;
			}
		}
		if(current == temp)//We have a GBoundaryV which is in orbit of barycentres' faces, and also have that the two triples are placed in reverse order.
		{
			order[0] = 1;
			order[1] = 0;
		}
		if(triplen[0][0] == triplen[current][2])
			break;
	}

	//Assigning neighbours:
	neighboursizes[cp] = 2*triplensize;//The third in each triple is part of overlap, thus total size of list is 2 times the number of triples.
	if(!(gp[cp].type == CombPoint::InteriorV || gp[cp].type == CombPoint::InteriorE))
		neighboursizes[cp]++;//In this case we can't come full circle with our list of neighbours so must have an extra slot for the last neighbour on the list.
	neighbours[cp] = new int[neighboursizes[cp]];
	for(int i = 0; i < triplensize; i++)
	{
		neighbours[cp][2*i] = triplen[order[i]][0];
		neighbours[cp][2*i+1] = triplen[order[i]][1];
	}
	if(!(gp[cp].type == CombPoint::InteriorV || gp[cp].type == CombPoint::InteriorE))
		neighbours[cp][neighboursizes[cp]-1] = triplen[order[triplensize-1]][2];//The last neighbour on the list.
}

void IGraph::getallneighbours(int **&neighbours, int *&neighboursizes)
{
	if(size <= 0)
		return;
	neighbours = new int*[size];
	neighboursizes = new int[size];

	//Setting neighbours of barycentres (obtained from faces):
	int fc = getfacecount();
	for(int i = 0; i < fc; i++)
	{
		neighbours[i+size-fc] = new int[face_sizes[i]];
		neighboursizes[i+size-fc] = face_sizes[i];
		for(int j = 0; j < face_sizes[i]; j++)
			neighbours[i+size-fc][j] = faces[i][j];
	}

	//Setting neighbours for remaining CombPoints:
	for(int i = 0; i < size-fc; i++)
		findneighbours(i, neighbours, neighboursizes);
}

int IGraph::getfacecount()
{
	int i = size-1;
	while(gp[i].type == CombPoint::Barycentre) i--;
	return (size-1) - i;
}

void IGraph::indexbarycentres()
{
	//Initialize our checklist:
	bool *inface = new bool[g.N];
	for(int i = 0; i < g.N; i++)
		inface[i] = false;

	//Points that are in outside-face orbit will be skipped, so they are ticked off are list here.
	int start = g.c[newdef];
	int point = start;
	do {
		inface[point] = true;
		point = g.c[point];
		point = g.e[point];
	} while(point != start);

	//Initializing face-related variables/arrays:
	faces = NULL;
	face_sizes = NULL;
	int facecount = 0;
	int *face = NULL;

	for(int i = 0; i < g.N; i++)
	{
		if(!inface[i])
		{
			int newfacesize = findface(face, i, inface);
			addface(face, newfacesize, facecount);
			addCP(-1, CombPoint::Barycentre);
		}
	}
}

void IGraph::info()
{
	for(int i = 0; i < size; i++)
	{
		cout << i << ": ";
		switch(gp[i].type) {
			case CombPoint::InteriorV:
				cout << "InteriorV ";
				break;
			case CombPoint::InteriorE:
				cout << "InteriorE ";
				break;
			case CombPoint::GBoundaryV:
				cout << "GBoundaryV ";
				break;
			case CombPoint::BoundaryV:
				cout << "BoundaryV ";
				break;
			case CombPoint::BoundaryE:
				cout << "BoundaryE ";
				break;
			case CombPoint::Barycentre:
				cout << "Barycentre ";
				break;
		};
		cout << gp[i].orbit << endl;
	}
	int fc = getfacecount();
	for(int i = 0; i < fc; i++)
	{
		cout << "Face #" << i+1 << ": ";
		for(int j = 0; j < face_sizes[i]; j++)
			cout << faces[i][j] << " ";
		cout << endl;
	}
}
