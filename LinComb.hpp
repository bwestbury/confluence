// 
//  LinComb.hpp
//  confluence
//  
//	Handles linear combinations.  Both the coefficients and base objects
//	are templated.  As yet, there is no simplification in addition.
//
//	This class relies on multiplication operations *= defined for the coefficients.
//
//  Created by David Howden on 2007-06-20.
//  Copyright 2007 David Howden. All rights reserved.
// 

#ifndef _LINCOMB_HPP_
#define _LINCOMB_HPP_

#include <iostream>
#include <iterator>
#include <utility>
#include <list>

using namespace std;

namespace Confluence
{

template <class E, class F>
class LinComb
{
public:	
	
	/*
		Typedef definitions of iterators
	*/
	typedef typename list< pair<E, F> >::iterator iterator;
	typedef typename list< pair<E, F> >::const_iterator const_iterator;
	
	
	/*
		Empty contructor
	*/
	LinComb() {}
	
	
	/*
		Construct a linear combination of one element, 
		with object E and coefficient F
		
	*/
	LinComb(E object, F coef) {
		push_back(object, coef);
	}
	
	
	~LinComb() {
		
	}
	
	
	/*
		Adds a term to the end of the linear combination
	*/
	void push_back(E object, F coef)
	{
		pair<E, F> p(object, coef);
		contents.push_back(p);
	}
	
	
	/*
		Add two linear combination together and return the result.
		
		NB: This does not simplify the result, it simply adds the
		two combinations as arrays.
	*/
	LinComb<E, F> operator+ (LinComb<E,F> &lc)
	{
		LinComb<E, F> result;
		
		// copy the items from this object
		result.contents = contents;
		
		// copy the items from lc using operator+=
		result += lc;
		
		// no simplification here yet!
		return result;
	}
	
	
	/*
		Add the given LinComb object to this one
		
		NB: this function simply appends the given LinComb object to
		this one.
	*/
	void operator+= (LinComb<E, F> &lc)
	{
		//contents.splice(contents.end(), lc.contents, lc.contents.begin(), lc.contents.end());
		for (iterator iter = lc.begin(); iter != lc.end(); iter++) {
			contents.push_back(*iter);
		}
	}
	
	
	/*
		Multiply this LinComb through by a coefficient F
		
		Returns the resulting LinComb object.
	*/
	LinComb<E, F> operator* (const F &coef)
	{
		LinComb<E, F> result;
		
		result.contents = contents;
		
		for(iterator iter = result.contents.begin(); iter != result.contents.end(); iter++) {
			iter->second *= coef;
		}
		return result;
	}
	
	
	/*
		Multiply this LinComb through by a coefficient F
		
		Stores the result in this object (ie *= operation)
	*/
	void operator*= (const F &coef)
	{
		for(iterator iter = contents.begin(); iter != contents.end(); iter++) {
			iter->second *= coef;
		}
	}
	
	
	/*
		Outputs this linear combination to the output stream os
		
	*/
	ostream &output(ostream &os)
	{
		iterator iter = contents.begin();
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
	
	
	/*
		Removes the first item from the LinComb object and returns it
		
	*/
	void pop_front()
	{
		contents.pop_front();
	}
	
	
	/*
		Checks if this LinComb object has any terms
		
	*/
	bool empty()
	{
		return contents.empty();
	}
	
	
	/*
		Returns the begin() iterator of the terms
		
	*/
	iterator begin()
	{
		return contents.begin();
	}
	
	const_iterator begin() const
	{
		return contents.begin();
	}
	
	
	/*
		Returns the end() iterator of the terms
		
	*/
	iterator end()
	{
		return contents.end();
	}
	
	const_iterator end() const
	{
		return contents.end();
	}
	
private:
	list< pair<E, F> > contents;
};

template<class E, class F>
ostream &operator<<(ostream &os, LinComb<E, F> &linComb) {
	return linComb.output(os);
}

}

#endif /* _LINCOMB_HPP_ */