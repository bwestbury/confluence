// 
//  SVGLineDiagram.hpp
//  confluence
//  
//  Created by David Howden on 2007-06-29.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _SVGLINEDIAGRAM_HPP_
#define _SVGLINEDIAGRAM_HPP_

#include <sstream>
#include <fstream>

using namespace std;

namespace Confluence {

class SVGLineDiagram
{
public:
	// SVGDiagram (int sw, int sh) : sizeWidth(sw), sizeHeight(sh), pixelWidth(0), pixelHeight(0) 
	// {
	// 	setup();
	// }
	
	SVGLineDiagram ()
	{
        sizeWidth = 10;
        sizeHeight = 10;
        pixelWidth = 500;
        pixelHeight = 500;
        setup();
	}
		
	SVGLineDiagram (int sw, int sh, int pw, int ph) : sizeWidth(sw), sizeHeight(sh), pixelWidth(pw), pixelHeight(ph) 
	{
		setup();
	}
	
	~SVGLineDiagram () {}
		
	void addLine(int x1, int y1, int x2, int y2)
	{
		content << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" stroke-width=\"2\"/>" << endl;
	}
	
	void saveToFile(char *filename)
	{
		ofstream fout(filename, ios::out);
		createFrame();
		fout << header.rdbuf();
		fout << content.rdbuf();
		fout << footer.rdbuf();
		fout.close();
	}

private:
	
	void setup()
	{
		header << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
		header << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	}
	
	void createFrame()
	{
		header << "<svg width=\"" << sizeWidth << "cm\" height=\"" << sizeHeight << "cm\" viewBox=\"0 0 " << pixelWidth << " ";
		header << pixelHeight << "\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << endl;
		header << "<rect x=\"1\" y=\"1\" width=\"" << (pixelWidth - 2) << "\" height=\"" << (pixelHeight - 2);
		header << "\" fill=\"none\" stroke=\"blue\" stroke-width=\"2\" />" << endl;
		footer << "</svg>";
	}
	
	int sizeWidth;
	int sizeHeight;
	int pixelWidth;
	int pixelHeight;
	
	stringstream header;
	stringstream content;
	stringstream footer;
};

}

#endif /* _SVGLINEDIAGRAM_HPP_ */
