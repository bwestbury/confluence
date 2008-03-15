// 
//  RewriteSystem.hpp
//  confluence
//
//	RewriteRule is the class used to store rules which are used to
//	reduce expressions.
//
//	Quite simply, it is a list of pairs matching one base element
//	to an equivalent linear combination of elements.
//
//  Created by David Howden on 2007-06-20.
//  Copyright 2007 David Howden. All rights reserved.
//

#ifndef _REWRITESYSTEM_HPP_
#define _REWRITESYSTEM_HPP_

#include <utility>
#include <list>

using namespace std;

namespace Confluence
{

template <class E, class F>
class RewriteSystem {
public:
	
	/*
		Iterator type definitions
		
	*/
	typedef typename list< pair<E, LinComb<E, F> > >::iterator iterator;
	typedef typename list< pair<E, LinComb<E, F> > >::const_iterator const_iterator;
	
	RewriteSystem() {}
	
	~RewriteSystem() {}
	
	/*
		Adds a rewrite rule to this rewrite system.
		
	*/
	void push_back(E lhs, LinComb<E, F> rhs)
	{
		pair<E, LinComb<E, F> > p(lhs, rhs);
		rules.push_back(p);
	}
	
	/*
		Performs Knuth Bendix completion on this set of
		rewrite rules.
		
		NB.  This function is awaiting implementation.
	*/
	void completeKnuthBendix() {
		
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
	
	void output(ostream &os)
	{
		for(const_iterator iter = begin(); iter != end(); iter++)
			os << iter->first << " --> " << iter->second << endl;
	}
	
private:
	list< pair<E, LinComb<E, F> > > rules;
};

template<class E, class F>
ostream &operator<< (ostream &os, RewriteSystem<E, F> &rs)
{
	rs.output(os);
	return os;
}

}

#endif /* _REWRITESYSTEM_HPP_ */