// 
//  LinCombTest.cpp
//  confluence
//  
//  Created by David Howden on 2007-07-08.
//  Copyright 2007 David Howden. All rights reserved.
// 

#include <iostream>
#include <string>

#include "Confluence.hpp"
#include "LinComb.hpp"

using namespace std;
using namespace Confluence;

class Test
{
public:
	Test (string n) : name(n) {}
	~Test () {}
	string getName() { return name; }
	// this is now required
	bool operator== (Test &t) { return (name == t.name); }
private:
	string name;
};

ostream& operator<<(ostream& os, Test &t) {
	os << t.getName();
	return os;
}

int main (int argc, char const *argv[])
{
	Test t1("Hello"), t2("World");
	
	
	// LinComb<Test, ex> lc1(t1, p1), lc2(t2, p2);
	LinComb<Test, int> lc1(t1, 1), lc2(t2, 2);
	
	cout << "lc1: " << lc1 << endl << "lc2: " << lc2 << endl;
	
	// LinComb<Test, ex> lc;
	LinComb<Test, int> lc;
	lc = lc1 + lc2;
	
	cout << "lc: " << lc << endl;
	cout << "lc1: " << lc1 << endl;
	cout << "lc2: " << lc2 << endl;
	LinComb<Test, int> lc3;
	lc3 = lc1 + lc1;
	cout << "lc1 + lc1" << lc3 << endl;
	
	return 0;
}
