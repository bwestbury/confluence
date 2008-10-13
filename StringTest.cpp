// 
//  StringTest.cpp
//  confluence
//  
//  Created by David Howden on 2008-03-14.
//  Copyright 2008 realbuzz.com Ltd. All rights reserved.
// 

#include <iostream>
#include <string>

#include "String.hpp"
#include "Confluence.hpp"

using namespace std;
using namespace Confluence;

int main (int argc, char const *argv[])
{
	// code testing find/replace
	// string test("This is a test"), find("test");
	// 	cout << test << endl;
	// 	// cout << find.length() << test.find(find) << endl;
	// 	test.replace(test.find(find), find.length(), "tester");
	// 	cout << test << endl;

	// ---
	//code testing String class
	String test("BAAB"), replace("D"), needle("AA");
	LinComb<String, int> replacement(replace, 3);
	replacement.push_back(replace, 4);
	cout << "Test String: " << test << endl;
	
	LinComb<String, int> result = test.searchReplace(needle, replacement);
	cout << "Result: " << result << endl;
	
	RewriteSystem<String, int> rw_system;
	rw_system.push_back(needle, replacement);
	
	String rule2_needle("BDB"), rule2_replace("Z");
	LinComb<String, int> rule2_replacement(rule2_replace, 100);
	rw_system.push_back(rule2_needle, rule2_replacement);
	
	LinComb<String, int> result2 = Simplify::simplifyElement(test, 1, rw_system);
	cout << endl << "Simplification: " << result2 << endl;
	// ---
	
	String ol1("ABC");
	String ol2("DEFADFSDFSDFSDFABCA");
	cout << endl << ol1 << endl << ol2 << endl;
	cout << "ol2.computeOverlap(ol1)" << endl << ol2.computeOverlap(ol1) << endl;
	cout << "ol2.computeOverlap(ol1)" << endl << ol1.computeOverlap(ol2) << endl;
	return 0;
}