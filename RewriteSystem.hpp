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
	
	typedef typename list< pair<E, LinComb<E, F> > >::iterator iterator;
	typedef typename list< pair<E, LinComb<E, F> > >::const_iterator const_iterator;
	
	RewriteSystem() {}
	
	~RewriteSystem() {}
	
	/*
		Adds a rewrite rule to this rewrite system.
		
	*/
	void addRewriteRule(E lhs, LinComb<E, F> rhs)
	{
		pair<E, LinComb<E, F> > p(lhs, rhs);
		rewriteRules.push_back(p);
	}
	
	/*
		Performs Knuth Bendix completion on this set of
		rewrite rules.
		
		NB.  This function is awaiting implementation.
	*/
	void completeKnuthBendix() {
		
	}
	
	iterator begin()
	{
		return rewriteRules.begin();
	}
	
	const_iterator begin() const
	{
		return rewriteRules.begin();
	}
	
	iterator end()
	{
		return rewriteRules.end();
	}
	
	const_iterator end() const
	{
		return rewriteRules.end();
	}
	
private:
	list< pair<E, LinComb<E, F> > > rewriteRules;
};

}

#endif /* _REWRITESYSTEM_HPP_ */