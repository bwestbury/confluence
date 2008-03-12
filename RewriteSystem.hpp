// 
//  RewriteSystem.hpp
//  confluence
//  
//  Created by David Howden on 2007-06-20.
//  Copyright 2007 David Howden. All rights reserved.
//

#ifndef _REWRITESYSTEM_HPP_
#define _REWRITESYSTEM_HPP_

#include <utility>
#include <list>

//#include "RewriteRule.hpp"

using namespace std;

namespace Confluence
{

template <class E, class F>
class RewriteSystem {
public:
	RewriteSystem() {
		
	}
	
	~RewriteSystem() {
		
	}
	
	void addRewriteRule(E lhs, LinComb<E, F> rhs) {
		pair<E, LinComb<E, F> > p(lhs, rhs);
		rewriteRules.push_back(p);
	}
	
	void completeKnuthBendix() {
		
	}
private:
	list< pair<E, LinComb<E, F> > > rewriteRules;
	//list< RewriteRule<E, F> > rewriteRules;
	// list<E> lhs;
	// list< LinComb<E, F> > rhs
};

}

#endif /* _REWRITESYSTEM_HPP_ */