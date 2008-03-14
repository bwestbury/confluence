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

using namespace std;
using namespace Confluence;

int main (int argc, char const *argv[])
{
	// string test("This is a test"), find("test");
	// 	cout << test << endl;
	// 	// cout << find.length() << test.find(find) << endl;
	// 	test.replace(test.find(find), find.length(), "tester");
	// 	cout << test << endl;
	String test("BAAB"), replace("D"), needle("AA");
	LinComb<String, int> replacement(replace, 3);
	cout << test;
	LinComb<String, int> result = test.searchReplace(needle, replacement);
	cout << result;
	return 0;
}