/*************************************************************

Copyright (c) 2006, Fernando Herrero Carrón 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.  
	* Redistributions in binary form must reproduce the above
	  copyright notice, this list of conditions and the following
	  disclaimer in the documentation and/or other materials provided
	  with the distribution. 
	* Neither the name of the author nor the names of his contributors
	  may be used to endorse or promote products derived from this
	  software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

$Id: TimeWrapper.h 337 2008-01-29 15:46:19Z elferdo $
*************************************************************/

#ifndef TIMEWRAPPER_H_
#define TIMEWRAPPER_H_

#include <concepts>
#include "IntegratedSystemConcept.h"

template <typename IntegratedSystem>
concept TimeWrapperConcept = IntegratedSystemConcept<IntegratedSystem> 
	&& requires(IntegratedSystem system, int substeps_per_cycle) {
	{ system.step(std::declval<typename IntegratedSystem::precision_t>()) };
	{ IntegratedSystem(system, substeps_per_cycle) };
};
class TimeWrapper : public IntegratedSystem
{	
	typedef typename IntegratedSystem::precision_t precision;
	
	int m_substeps_per_cycle;
	
	IntegratedSystem &m_system;

public:

	typedef typename IntegratedSystem::ConstructorArgs ConstructorArgs;
	typedef typename IntegratedSystem::variable variable;
	typedef typename IntegratedSystem::parameter parameter;

	TimeWrapper(IntegratedSystem &wrapee, int substeps_per_cycle = 1) : m_substeps_per_cycle(substeps_per_cycle), m_system(wrapee)
	{
	}
	
	void step(precision h)
	{
		for(int i = 0; i < m_substeps_per_cycle; i++){
			m_system.step(h);
		}
	}
};

#endif /*TIMEWRAPPER_H_*/
