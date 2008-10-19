/*
	Graph drawing code

	Written by Sebastian Jörn.
	Seperated by David Howden.
 */

#include "drawing.hpp"

void Graph_output(Graph &g, int *boundary)
{
	//Configure with boundary:
	Graph gnew = IGraph::configure(g, boundary);

	//Index the configured Graph:
	IGraph ig(gnew, g.N);
	ig.info();

	//Find the neighbours of each IGCombPoint in the Graph:
	int **neighbours;
	int *neighboursizes;
	ig.getallneighbours(neighbours, neighboursizes);

	//Now create a labeled complex:
	LComplex lc(ig, neighbours, neighboursizes);

	//Finally, perform circle packing and layout:
	lc.make_packing();
	lc.layout();
}
