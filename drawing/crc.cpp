// 
//  crc.cpp
//  
//  Created by Sebastian Jörn on 2008-07-30.
//  Copyright 2008 Sebastian Jörn. All rights reserved.
// 

#include "crc.hpp"

LComplex::LComplex(CombPoint *cp, int s,  int **n, int *ns): size(s), packing(false)
{	
	neighbours = new int*[size];
	nsizes = new int[size];
	label = new long double[size];
	c = new CombPoint[size];
	for(int i = 0; i < size; i++)
	{
		nsizes[i] = ns[i];
		c[i] = cp[i];
		neighbours[i] = new int[nsizes[i]];
		for(int j = 0; j < nsizes[i]; j++)
			neighbours[i][j] = n[i][j];
		label[i] = 1;//Assigning arbitrary value as a putative radius.
	}
}

LComplex::LComplex(IGraph &ig, int **n, int *ns): size(ig.getsize()), packing(false)
{
	neighbours = new int*[size];
	nsizes = new int[size];
	label = new long double[size];
	c = new CombPoint[size];
	for(int i = 0; i < size; i++)
	{
		nsizes[i] = ns[i];
		c[i] = ig[i];
		neighbours[i] = new int[nsizes[i]];
		for(int j = 0; j < nsizes[i]; j++)
			neighbours[i][j] = n[i][j];
		label[i] = 1;//Assigning arbitrary value as a putative radius.
	}
}

LComplex::~LComplex()
{
	delete [] c;
	delete [] nsizes;
	delete [] label;
	for(int i = 0; i < size; i++)
		delete [] neighbours[i];
	delete [] neighbours;
}

long double LComplex::angle_sum(int circle)
{
	long double temp;

	int k =nsizes[circle];
	long double *r = new long double[k+1];
	r[0] = label[circle];
	for(int i = 0; i < k; i++)
		r[i+1] = label[neighbours[circle][i]];

	//Calculate the angles for all the triples (in Stephenson's sense):
	long double *alpha = new long double[k];
	for(int j = 1; j < k; j++)//Law of Cosines
	{
		try {
			alpha[j-1] = angle(r[0], r[j], r[j+1]);
			if(!(alpha[j-1] == 0 || alpha[j-1] < 0 || alpha[j-1] > 0))
				throw "Indefinite number!";
		}
		catch(char *str) {
			cout << "Exception raised: " << str << endl;
			cout << "r[0] = " << r[0] << "\t" << "r[" << j << "] = " << r[j] << "\t" << "r[" << j+1 << "] = " << r[j+1] << endl;
			cin.get();
		}
	}
	//Tie the bag together (will not be done for BoundaryV, GBoundaryV and BoundaryE):
	if(c[circle].type == CombPoint::InteriorV || c[circle].type == CombPoint::InteriorE || c[circle].type == CombPoint::Barycentre)
	{
		try {
			alpha[k-1] = angle(r[0], r[1], r[k]);
			if(!(alpha[k-1] == 0 || alpha[k-1] < 0 || alpha[k-1] > 0))
				throw "Indefinite number!";
		}
		catch(char *str) {
			cout << "Exception raised: " << str << endl;
			cout << "r[0] = " << r[0] << "\t" << "r[" << k << "] = " << r[k] << "\t" << "r[" << 1 << "] = " << r[1] << endl;
			cin.get();
		}
	}
	
	//Add all the angles together:
	long double theta = alpha[0];
	for(int i = 1; i < k-1; i++)
		theta += alpha[i];
	if(c[circle].type == CombPoint::InteriorV || c[circle].type == CombPoint::InteriorE || c[circle].type == CombPoint::Barycentre)
		theta += alpha[k-1];

	delete [] alpha;
	delete [] r;

	return theta;
}

long double LComplex::distance()
{
	long double excess = 0.0, shortage = 0.0;
	for(int i = 0; i < size; i++)
	{
		long double theta = angle_sum(i);
		long double targetsum = target_sum(i);;
		if(targetsum - theta < 0)
			excess += theta - targetsum;
		else
			shortage += targetsum - theta;
	}
	long double dis = excess + shortage;
	cout << "Current distance: " << dis << endl << endl << endl;
	return dis;
}

void LComplex::info()
{
	cout << "INFORMATION ON THE LABELED COMPLEX" << endl;
	if(packing)
		cout << "Status: PACKING" << endl;
	else
		cout <<"Status: NOT A PACKING" << endl;
	cout << "List of CombPoints:" << endl;
	for(int i = 0; i < size; i++)
	{
		cout << "\t" << i << ": ";
		switch(c[i].type) {
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
		cout << "\t" << "Label: " << label[i] << endl << "\tNeighbours: ";
		for(int j = 0; j < nsizes[i]; j++)
			cout << neighbours[i][j] << " ";
		cout << endl << endl;
	}
}

bool LComplex::ispacking()
{
	long double epsilon = margin_of_error;
	if(distance() < epsilon)
		packing = true;
	return packing;
}

void LComplex::layout()//Based on the meta-code in Stephenson's book for simply connected complexes.
{
	if(packing)
	{
		//Initialize variables:
		long double (*coordinates)[2] = new long double[size][2];//Coordinates (x,y) of each circle.
		bool *placed = new bool[size];//Our check-list for whether a circle has been placed.
		bool allplaced = false;
		for(int i = 0; i < size; i++)
			placed[i] = false;

		//Place first two circles: a BoundaryV at the origin and one of its neighbouring edges on positive x-axis.
		for(int i = 0; i < size; i++)
		{
			if(c[i].type == CombPoint::BoundaryV)
			{
				coordinates[i][0] = 0;
				coordinates[i][1] = 0;
				placed[i] = true;

				coordinates[neighbours[i][0]][0] = label[i] + label[neighbours[i][0]];
				coordinates[neighbours[i][0]][1] = 0;
				placed[neighbours[i][0]] = true;

				break;
			}
		}

		//Place remaining circles:
		do {
			allplaced = true;//Assume all are placed...
			for(int i = 0; i < size; i++)//i is v in Stephenson's meta-code.
			{
				if(!placed[i])
				{
					allplaced = false;//...and correct that assumption if not.
					for(int j = 0; j < nsizes[i]-1; j++)
					{ 
						if(placed[neighbours[i][j]] && placed[neighbours[i][j+1]])//if u and w are placed...
						{
							place_circle(i, neighbours[i][j], neighbours[i][j+1], coordinates, placed);
							break;
						}
					}
					//Tie the bag together (will not be done for BoundaryV, GBoundaryV and BoundaryE) [i.e. a final check after the for-loop to come full circle]:
					if((c[i].type == CombPoint::InteriorV || c[i].type == CombPoint::InteriorE || c[i].type == CombPoint::Barycentre) && !placed[i])//Last check needed since could have been placed in previous if-statement.
					{
						if(placed[neighbours[i][nsizes[i]-1]] && placed[neighbours[i][0]])
						{
							place_circle(i, neighbours[i][nsizes[i]-1], neighbours[i][0], coordinates, placed);
						}
					}
				}
			}
			//If all are placed, the program will not have gone into "if(!placed[i])", and thus, by our initial assumption, allplaced will remain true.
		} while(!allplaced);

		//Now have all coordinates, so all we need to do now is draw! We do this by using Fourey's BoardLib.
		layout_draw(coordinates, placed);
	}
	else
		cout << "Not a packing!" << endl;
}

void LComplex::layout_draw(long double (*coordinates)[2], bool *placed)
{
	Board board;
	unsigned char circles_lum = 220, boundary_lum = 0;
	unsigned char graph_red = 255, graph_green = 0, graph_blue = 0;
	Color circles_color(circles_lum);
	Color boundary_color(boundary_lum);
	Color graph_color(graph_red, graph_green, graph_blue);

	cout << endl << "Draw circles? (y,n): ";
	if(cin.get() == 'y')
	{
		//Draw circles:
		board.setPenColor(circles_color);
		for(int i = 0; i < size; i++)
			if(placed[i])
				board.drawCircle(coordinates[i][0], coordinates[i][1], label[i]);
	}

	//Draw boundary:
	board.setPenColor(boundary_color);
	for(int i = 0; i < size; i++)
	{
		if(c[i].type == CombPoint::BoundaryV)
		{
			int index = i;
			do {
				board.drawLine(coordinates[index][0], coordinates[index][1], coordinates[neighbours[index][0]][0], coordinates[neighbours[index][0]][1]);
				index = neighbours[index][0];
			} while (index != i);
			break;
		}
	}

	//Draw graph:
	board.setPenColor(graph_color);
	int index = 0;
	int barycentres = getnum_barycentres();;
	while(index < size - barycentres)
	{
		for(int i = 0; i < nsizes[index]; i++)
		{
			if(c[neighbours[index][i]].type == CombPoint::InteriorE)
			{
				board.drawLine(coordinates[index][0], coordinates[index][1], coordinates[neighbours[index][i]][0], coordinates[neighbours[index][i]][1]);
				board.fillCircle(coordinates[index][0], coordinates[index][1], 0.05);//Draws a point at the Graph-vertex.
			}
		}
		index++;
	}

	board.saveSVG("graph.svg");
}

void LComplex::make_packing()
{
	while(!ispacking())
	{
		int N = number_of_iterations;
		for(int i = 0; i < N; i++)
			for(int j = 1; j < size; j++)//Skipping the first one to keep it fixed (scaling factor).
				uniformneighbour_model(j);
	}
	//info();
}

void LComplex::place_circle(int v, int u, int w, long double (*&coordinates)[2], bool *&placed)
{
	//cout << u << " and " << w << " are placed. Placing " << v << "..." << endl;
	long double r = label[u], r1 = label[w], r2 = label[v];

	double xtemp = coordinates[w][0] - coordinates[u][0];
	double ytemp = coordinates[w][1] - coordinates[u][1];
	double beta;
	if(xtemp > 0 && ytemp > 0)
		beta = acos(xtemp/(r+r1));
	else if(xtemp < 0 && ytemp > 0)
		beta = pi - acos((-xtemp)/(r+r1));
	else if(xtemp < 0 && ytemp < 0)
		beta = pi + acos((-xtemp)/(r+r1));
	else if(xtemp > 0 && ytemp < 0)
		beta = 2*pi - acos(xtemp/(r+r1));
	else if(xtemp == 0 && ytemp > 0)
		beta = pi/2;
	else if(xtemp == 0 && ytemp < 0)
		beta = 3*pi/2;
	else if(ytemp == 0 && xtemp < 0)
		beta = pi;
	else// if(ytemp == 0 && xtemp > 0)
		beta = 0;
	//Usage of values of xtemp, ytemp ends here;

	long double alpha = angle(r, r1, r2);

	//Calculating x,y assuming u lies at origin and v lies on +ve x-axis.
	long double x = r + r2;
	long double y = 0;
	//Rotate coordinates (thus getting w at correct angle in relation to u, but u still at origin):
	xtemp = x; ytemp = y;
	x = xtemp*cos(alpha + beta) - ytemp*sin(alpha + beta);
	y = xtemp*sin(alpha+beta) + ytemp*cos(alpha+beta);

	//Move origin (thus placing both u and w at original coordinates):
	x += coordinates[u][0];
	y += coordinates[u][1];

	//Enter new coordinates:
	coordinates[v][0] = x;
	coordinates[v][1] = y;
	placed[v] = true;
}

long double LComplex::target_sum(int circle)
{
	switch(c[circle].type) {
		case CombPoint::BoundaryE:
		case CombPoint::GBoundaryV:
			return pi;
		case CombPoint::BoundaryV:
			return target_sum_BoundaryV();
		case CombPoint::None:
			return 0;
		default:
			return 2*pi;
	};
}

long double LComplex::target_sum_BoundaryV()
{
	int counter = 0;
	for(int i = 0; i < size; i++)
		if(c[i].type == CombPoint::BoundaryV)
			counter++;
	return (counter-2)*pi/counter;
}

void LComplex::uniformneighbour_model(int circle)
{
	int k = nsizes[circle];
	long double theta = angle_sum(circle);
	long double beta = sin(theta/(2*k)), delta = sin(target_sum(circle)/(2*k));
	long double r_hat = (beta/(1-beta))*label[circle];
	long double ro = ((1-delta)/delta)*r_hat;
	label[circle] = ro;
}


long double angle(long double r, long double r1, long double r2)
{
	return acos((pow(r+r1,2) + pow(r+r2,2) - pow(r1+r2,2) ) / (2*(r+r1)*(r+r2)));
}

