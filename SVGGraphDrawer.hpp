// 
//  SVGGraphDrawer.hpp
//  confluence
//  
//  Created by David Howden on 2007-08-20.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _SVGGRAPHDRAWER_HPP_
#define _SVGGRAPHDRAWER_HPP_

#include <iterator>
#include <map>

#include "SVGLineDiagram.hpp"
#include "GraphDrawer.hpp"

using namespace std;

namespace Confluence {

class SVGGraphDrawer : public GraphDrawer
{
public:   
    SVGGraphDrawer() {}
    ~SVGGraphDrawer() {}

    void outputGraphDiagram(char *filename)
    {
        SVGLineDiagram ld;
        int size = x1.size();
        for(int i = 0; i < size; i++) {
            ld.addLine(x1[i], y1[i], x2[i], y2[i]);
        }
        ld.saveToFile(filename);
    }
};

}

#endif /* _SVGGRAPHDRAWER_HPP_ */
