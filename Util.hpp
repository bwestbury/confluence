// 
//  Util.hpp
//  confluence
//  
//  Created by David Howden on 2007-06-27.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <iostream>
#include <iterator>
#include <fstream>
#include <set>

using namespace std;

namespace Confluence
{

template<class T>
ofstream& operator<<(ofstream &os, const set< T > &s) {
	copy(s.begin(), s.end(), ostream_iterator<T>(os, " "));
	return os;
}

template<class T>
ostream& operator<<(ostream &os, const set< T > &s) {
	os << "{";
	copy(s.begin(), s.end(), ostream_iterator<T>(os, ", "));
	os << "}";
	return os;
}

}

#endif /* _UTIL_HPP_ */
