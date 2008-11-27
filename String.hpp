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
using namespace Confluence;

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
	
	String computeOverlap (const String &str)
	{
	    if (str.s.size() > s.size())
            return String();
        
        string::const_iterator iter1, iter2, str_iter;
        iter1 = s.begin();
        
        for(iter1 = s.begin(); iter1 != s.end(); iter1++)
        {
            iter2 = iter1;
            for (str_iter = str.s.begin(); str_iter != str.s.end(); str_iter++) {
                if (*iter2 != *str_iter) {
                    break;
                }
                iter2++;
            }
            if (str_iter == str.s.end()) {
                return str;
            }
        }
        
        return String();
	}
	
	
    // String computeOverlap (const String &str)
    // {
    //  int i = 0;
    //  int n = str.s.size();
    //  
    //  if (n > s.size())
    //      n = s.size();
    //  
    //  string::const_iterator str_iter = str.s.end();
    //  string::const_iterator iter = s.begin();
    //  
    //  str_iter--;
    //  
    //  cout << "overlap: " << (*str_iter) << " " << *iter << endl;
    //  
    //  while((*str_iter) == (*iter)) {
    //      cout << "overlap: " << (*str_iter) << endl;
    //      str_iter--;
    //      iter++;
    //  }
    //  
    //  while (s[i] == str.s[str.s.size()-i-1] && i < n) {
    //      i++;
    //  }
    //  
    //  cout << i << endl;
    //  
    //  String ret;
    //  if (i > 0) {
    //      ret.s = str.s.substr(0, str.s.size()-i)+s;
    //  }
    //  
    //  return ret;
    // }
	
	bool operator>= (const String &str)
	{
		return s >= str.s;
	}
	
	bool operator> (const String &str)
	{
		return s > str.s;
	}
	
	bool operator< (const String &str)
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

#endif /* _STRING_HPP_ */
