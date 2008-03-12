// 
//  String.hpp
//  confluence
//  
//  Created by David Howden on 2007-06-22.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <string>

#include "Confluence.hpp"

using namespace std;

namespace Confluence
{

template<class F>
class String
{
public:
	String() {}
	String(string str) {
		s = str;
	}
	~String() {}
	
	bool operator==(const String &str) {
		return (s == str.s);
	}
	
	LinComb<String, F> searchReplace(const String &needle, const LinComb<String,F> &replacement) {
		return NULL;
	}
private:
	string s;
};

}

#endif /* _STRING_HPP_ */
