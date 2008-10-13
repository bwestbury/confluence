#include "crc.h"

void test()//Testing configuration, indexing and circle packing on premade polygons glued together.
{
	//Given original Graph:
	Graph g;
	g = g.createPolygon(3);
	Graph g1 = g.createPolygon(4);
	Graph g2 = g.createPolygon(5);
	Graph g3 = g1.glue(g, g1, 1);
	Graph g4 = g1.glue(g3, g2, 1);
	Graph g5 = g1.glue(g4, g, 2);

	//Configure with boundary:
	int boundary[7] = {1,0,1,0,2,0,0};

	Graph_output(g5, boundary);
}

int main()
{
	test();
	//cin.get();
	return 0;
}