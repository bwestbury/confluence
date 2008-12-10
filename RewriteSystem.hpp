// 
//	 RewriteSystem.hpp
//	 confluence
//
//	 RewriteRule is the class used to store rules which are used to
//	 reduce expressions.
//
//	 Quite simply, it is a list of pairs matching one base element
//	 to an equivalent linear combination of elements.
//
//	 Created by David Howden on 2007-06-20.
//	 Copyright 2007 David Howden. All rights reserved.
//

#ifndef _REWRITESYSTEM_HPP_
#define _REWRITESYSTEM_HPP_

#include <list>
#include <utility>

#include "LinComb.hpp"

using namespace std;

namespace Confluence
{

template <class F, class T, F FZERO>
class RewriteSystem
{
public:

	/*
		RewriteRule type definitions
	*/
	typedef pair<T, LinComb<F, T, FZERO> > rewriteRule;


	/*
		Iterator type definitions
	*/
	typedef typename list<rewriteRule>::iterator iterator;
	typedef typename list<rewriteRule>::const_iterator const_iterator;

	/*
		Empty contructor
	*/
	RewriteSystem()
	{
		
	}


	/*
		Destructor
	*/
	~RewriteSystem()
	{
		
	}
	
	
	/*
		Adds a rewrite rule to this rewrite system.
	*/
	void push_back(T lhs, LinComb<F, T, FZERO> rhs)
	{
		rewriteRule p(lhs, rhs);
		rules.push_back(p);
	}


	/*
		Performs Knuth Bendix completion on this set of
		rewrite rules.
	
		NB.  This function is awaiting implementation.
	*/
	void completeKnuthBendix()
	{
		
	}


	/*
		Iterator for the first rewriteRule
	*/
	iterator begin()
	{
		return rules.begin();
	}


	/*
		Constant iterator for the first rewriteRule
	*/
	const_iterator begin() const
	{
		return rules.begin();
	}


	/*
		Iterator for the last rewriteRule
	*/
	iterator end()
	{
		return rules.end();
	}


	/*
		Constant iterator for the last rewriteRule
	*/
	const_iterator end() const
	{
		return rules.end();
	}


	/*
		Output the RewriteSystem to the ostream os
	*/
	void output(ostream &os)
	{
		for(const_iterator iter = begin(); iter != end(); iter++) {
			os << iter->first << " --> " << iter->second << endl;
		}
	}
	
private:
	/*
		List to store the rewrite rules for the rewrite system
	*/
	list<rewriteRule> rules;
};

}	/* Confluence */


template<class F, class T, F FZERO>
ostream &operator<< (ostream &os, Confluence::RewriteSystem<F, T, FZERO> &rs)
{
	rs.output(os);
	return os;
}

#endif  /* _REWRITESYSTEM_HPP_ */
