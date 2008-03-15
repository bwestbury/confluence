// 
//  Simplify.hpp
//  confluence
//  
//  Created by David Howden on 2008-03-15.
//  Copyright 2008 David Howden All rights reserved.
// 

#ifndef _SIMPLIFY_HPP_
#define _SIMPLIFY_HPP_

#include "LinComb.hpp"
#include "RewriteSystem.hpp"

namespace Confluence
{

class Simplify
{
	// friend class RewriteSystem<E, F>;
	// friend class LinComb<E, F>;
public:
	
	/*
		Reduces a single element, given by coefficient F and base E, and returns
		the resulting linear combination.
		
		By definition, this linear combination should only have one term in.
	*/
	template <class E, class F>
	static LinComb<E, F> simplifyElement(const E base, const F coeff, const RewriteSystem<E, F> &rewriteSystem) {
		LinComb<E, F> result(base, coeff);
		simplifyElement(result, rewriteSystem);
		return result;
	}
	
	
	/*
		Reduces a linear combination to its minimal form using a rewrite system.
		
		NB: This function actually acts on base, and so base becomes the reduced form
	*/
	template <class E, class F>
	static void simplifyElement(LinComb<E, F> &base, const RewriteSystem<E, F> &rewriteSystem) {
		
		LinComb<E, F> simplifiedComb;
		
		bool flag = true;

		//  loop through all of the base elements
		for(typename LinComb<E, F>::iterator objectIter = base.begin(); objectIter != base.end(); objectIter++) {
			while(flag) {
				flag = false;
				// loop through all of the rewrite rules
				for(typename RewriteSystem<E, F>::const_iterator ruleIter = rewriteSystem.begin(); ruleIter != rewriteSystem.end(); ruleIter++) {
					// apply the rule to the base element, which returns a linear combination which
					// is equivalent to (*objectIter).

					simplifiedComb = objectIter->first.searchReplace(ruleIter->first, ruleIter->second);

					// is the list empty (ie no simplification done)
					if(!simplifiedComb.empty()) {
						// the list isn't empty

						// now need to multiply through by the coefficient of objectIter (which is
						// objectIter->second)
						simplifiedComb *= objectIter->second;

						// the first element in the linear combination is going to be the modified
						// (*objectIter), so we need to replace the current one with this one...
						(*objectIter) = (*(simplifiedComb.begin()));
						// now remove the element from the front of the list
						simplifiedComb.pop_front();

						// the new objects are added to the end of the list to be simplified later
						base += simplifiedComb;

						// and since we have simplified the expression, we need to mark the flag
						flag = true;
					}
				}				
			}
		}
	}
};

}

#endif /* _SIMPLIFY_HPP_ */
