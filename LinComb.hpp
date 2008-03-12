// 
//  LinComb.hpp
//  confluence
//  
//  Created by David Howden on 2007-06-20.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _LINCOMB_HPP_
#define _LINCOMB_HPP_

#include <iostream>
#include <utility>
#include <list>

using namespace std;

namespace Confluence
{

template <class E, class F>
class LinComb
{
public:
	LinComb() {}
	
	LinComb(E object, F coef) {
		pair<E, F> p(object, coef);
		contents.push_back(p);
	}
	
	~LinComb() {
		
	}
	
	LinComb<E, F> operator+ (LinComb<E,F> &lc)
	{
		LinComb<E, F> result;
		
		// copy the items from this object
		result.contents = contents;
		
		// copy the items from lc
		result.contents.splice(result.contents.end(), lc.contents, lc.contents.begin(), lc.contents.end());
		
		// no simplification here yet!
		return result;
	}
	
	LinComb<E, F> operator* (const F &coef)
	{
		LinComb<E, F> result;
		
		result.contents = contents;
		
		for(typename list< pair<E,F> >::iterator iter = result.contents.begin(); iter != result.contents.end(); iter++) {
			iter->second*=coef;
		}
		return result;
	}
	
	void operator*= (const F &coef)
	{
		for(typename list< pair<E,F> >::iterator iter = contents.begin(); iter != contents.end(); iter++) {
			iter->second *= coef;
		}
	}
	
	ostream &output(ostream &os)
	{
		typename list< pair<E,F> >::iterator iter = contents.begin();
		while(true) {
			os << iter->first << "*" << iter->second;
			iter++;
			if(iter != contents.end()) {
				os << " + ";
			} else {
				break;
			}
		}
		return os;
	}
	
private:
	list< pair<E,F> > contents;
};

template<class E, class F>
ostream &operator<<(ostream &os, LinComb<E, F> &linComb) {
	return linComb.output(os);
}

}

#endif /* _LINCOMB_HPP_ */