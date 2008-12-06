// 
//  LinComb.hpp
//  confluence
//  
//	 Handles linear combinations.  Both the coefficients and base objects
//	 are templated.  As yet, there is no simplification in addition.
//
//	 This class relies on multiplication operations *= defined for the coefficients.
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
		Typedef definitions for pairs
	*/
	typedef pair<E, F> monomial;
	
	
	/*
		Typedef definitions of iterators
	*/
	typedef typename list< monomial >::iterator iterator;
	typedef typename list< monomial >::const_iterator const_iterator;
	
	
	/*
		Empty contructor
	*/
	LinComb() {}
	
	
	/*
		Construct a linear combination of one element, 
		with object E and coefficient F
	*/
	LinComb(E object, F coef)
	{
		push_back(object, coef);
	}


	/*
		Destructor
	*/
	~LinComb() {}


	/*
		Adds a term to the end of the linear combination
	*/
	void push_back(E object, F coef)
	{
		monomial p(object, coef);
		push_back(p);
	}
	
	
	/*
		Adds a term to the end of the linear combination.
		
		If the object being added is "==" to one already in the list, then the coeffs
		are added.
	*/
	void push_back(monomial p)
	{
		bool found = false;
		for (iterator iter = begin(); iter != end(); iter++) {
			// check to see if there is an object "==" to the one being added
			if (p.first == iter->first) {
				// if so, add the coeffs
				iter->second += p.second;
				found = true;
				break;
			}
		}
		// check if the object was "==" to one already in the list
		if (!found) {
			// nope, add it to the end...
			contents.push_back(p);
		}
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


	/*
		Returns the begin() cont_iterator of the terms
	*/
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


	/*
		Returns the begin() const_iterator of the terms
	*/
	const_iterator end() const
	{
		return contents.end();
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
			push_back(*iter);
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
		Evaluates (this) < (lc) and returns the result.
	*/
	bool operator< (const LinComb<E, F> &lc)
	{
		if (contents.size < lc.contents.size) {
			return true;
		}
	
		// TODO:  add more code here!
	
		return false;
	}


	/*
		Evaluates (this) > (lc) and returns the result.
	*/
	bool operator> (const LinComb<E, F> &lc)
	{
		return lc < this;
	}


	/*
		Computes the overlap of (this) and (lc) and returns the result
		as a new LinComb
	*/
	LinComb<E, F> computeOverlap (LinComb<E, F> &lc)
	{
	
	}


	/*
		Outputs (this) LinComb to the output stream os
	*/
	ostream& output (ostream &os)
	{
		if (empty()) {
			os << "0";
		} else {
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
		}
		return os;
	}
	
private:
	list< monomial > contents;
};

} /* Confluence */

template<class E, class F>
ostream &operator<<(ostream &os, Confluence::LinComb<E, F> &linComb) {
	return linComb.output(os);
}

#endif  /* _LINCOMB_HPP_ */