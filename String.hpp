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

class String
{
public:
	String() {}
	String(string str) {
		s = str;
	}
	~String() {}
	
	bool operator==(const String &str)
	{
		return (s == str.s);
	}
	
	template<class F>
	LinComb<String, F> searchReplace(const String &needle, const LinComb<String,F> &replacement)
	{
		LinComb<String, F> result;
		
		size_t position = s.find(needle.s);
		if (position == string::npos)
			return result;
		
		for (typename LinComb<String, F>::const_iterator iter = replacement.begin(); iter != replacement.end(); iter++) {
			string s_copy = s;
			result.push_back(s_copy.replace(position, needle.s.length(), iter->first.s), iter->second);
		}
		
		return result;
	}
	
		
	String computeOverlap(const String &str)
	{
		
	}
	
	bool operator>=(const String &str)
	{
		return s >= str.s;
	}
	
	bool operator>(const String &str)
	{
		return s > str.s;
	}
	
	bool operator<(const String &str)
	{
		return s < str.s;
	}
	
	
	ostream &output(ostream& os)
	{
		os << s;
		return os;
	}
	
private:
	string s;
};

ostream &operator<< (ostream& os, String str)
{
	str.output(os);
	return os;
}

}

#endif /* _STRING_HPP_ */
