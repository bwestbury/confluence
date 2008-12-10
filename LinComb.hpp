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

/*
	Linear combinations with:
	-	F as the field class
	-	T as the object class
	-	FZERO as the field object representing 0
*/
template <class F, class T, F FZERO>
class LinComb
{
public:
	
	/*
		Typedef definitions for pairs
	*/
	typedef pair<F, T> monomial;
	
	
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
	LinComb(F coef, T object)
	{
		push_back(coef, object);
	}


	/*
		Destructor
	*/
	~LinComb() {}


	/*
		Adds a term to the end of the linear combination
	*/
	void push_back(F coef, T object)
	{
		monomial p(coef, object);
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
			if (p.second == iter->second) {
				// if so, add the coeffs
				iter->first += p.first;
				if (iter->first == FZERO) {
					// remove the monomial from the linear combination
					contents.erase(iter);
				}
				found = true;
				break;
			}
		}
		// check if the object was "==" to one already in the list
		if (!found) {
			// nope, add it to the end...
			_push_back(p);
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
	LinComb<F, T, FZERO> operator+ (LinComb<F, T, FZERO> &lc)
	{
		LinComb<F, T, FZERO> result;
	
		// copy the items from this object
		result.contents = contents;
	
		// copy the items from lc using operator+=
		result += lc;
	
		// no simplification here yet!
		return result;
	}


	/*
		Add the given LinComb object to this one.
		
		Modifies this object.
	*/
	void operator+= (LinComb<F, T, FZERO> &lc)
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
	LinComb<F, T, FZERO> operator* (const F &coef)
	{
		LinComb<F, T, FZERO> result;
		
		if (coef == FZERO) {
			return result;
		}
		
		result.contents = contents;
	
		for(iterator iter = result.begin(); iter != result.end(); iter++) {
			iter->first *= coef;
		}
		return result;
	}


	/*
		Multiply this LinComb through by a coefficient F
	
		Stores the result in this object (ie *= operation)
	*/
	void operator*= (const F &coef)
	{
		if (coef == FZERO) {
			contents.empty();
			return;
		}
		
		for(iterator iter = contents.begin(); iter != contents.end(); iter++) {
			iter->first *= coef;
		}
	}


	/*
		Subtract the given LinComb object from this one
		
		Modifies this object
	*/
	void operator-= (LinComb<F, T, FZERO> &lc)
	{
		for (iterator iter = lc.begin(); iter != lc.end(); iter++) {
			push_back(monomial(FZERO - iter->first, iter->second));
		}
	}


	/*
		Subtract the given LinComb object from this one
		
		Returns the result in a new object
	*/
	LinComb<F, T, FZERO> operator- (LinComb<F, T, FZERO> &lc)
	{
		LinComb<F, T, FZERO> result;
		result.contents = contents;
		result -= lc;
		return result;
	}


	/*
		Divide all the coefficients in this object by the given coef
		
		Modified this object
	*/
	void operator/= (const F &coef)
	{
		for (iterator iter = begin(); iter != end(); iter++) {
			iter->first /= coef;
		}
	}


	/*
		Divide all the coefficients in this object by the given coef
		
		Returns the result in a new object
	*/
	LinComb<F, T, FZERO> operator/ (const F &coef)
	{
		LinComb<F, T, FZERO> result;
		result.contents = contents;
		result /= coef;
		return result;
	}


	/*
		Returns the coef of the given object in this linear combination.
	*/
	F find_object_coef(T object)
	{
		for (iterator iter = begin(); iter != end(); iter++) {
			if (iter->second == object) {
				return iter->first;
			}
		}
		return FZERO;
	}

	// /*
	// 	Evaluates (this) < (lc) and returns the result.
	// */
	// bool operator< (const LinComb<F, T, FZERO> &lc)
	// {
	// 	if (contents.size < lc.contents.size) {
	// 		return true;
	// 	}
	// 
	// 	// TODO:  add more code here!
	// 
	// 	return false;
	// }
	// 
	// 
	// /*
	// 	Evaluates (this) > (lc) and returns the result.
	// */
	// bool operator> (const LinComb<F, T, FZERO> &lc)
	// {
	// 	return lc < this;
	// }
	// 
	// 
	// /*
	// 	Computes the overlap of (this) and (lc) and returns the result
	// 	as a new LinComb
	// */
	// LinComb<E, F> computeOverlap (LinComb<F, T, FZERO> &lc)
	// {
	// 
	// }


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
	/*
		List to store the monomials to form linear combinations.
	*/
	list< monomial > contents;


	/*
		Internal function for adding monomial to the end this linear
		combination.
		
		Should only be used when it is know that the resulting expression
		is reduced.
	*/
	void _push_back(monomial p)
	{
		contents.push_back(p);
	}
};

} /* Confluence */


template<class F, class T, F FZERO>
ostream &operator<<(ostream &os, Confluence::LinComb<F, T, FZERO> &linComb) {
	return linComb.output(os);
}

#endif  /* _LINCOMB_HPP_ */