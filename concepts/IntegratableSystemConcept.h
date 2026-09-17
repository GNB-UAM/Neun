/*************************************************************

Copyright (c) 2007, Fernando Herrero Carrón 
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

$Id: IntegratableSystemConcept.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef INTEGRATABLESYSTEMCONCEPT_H_
#define INTEGRATABLESYSTEMCONCEPT_H_

#include <concepts>
#include "SystemConcept.h"

/* \class IntegratableSystemConcept
 * 
 * A model of this concept must meet the requirements for SystemConcept plus:
 * 
 * The following methods
 * \li void pre_step(precision_t h)
 * \li void post_step(precision_t h)
 * 
 * The following static members
 * \li precision_t* m_variables
 * \li precision_t* m_parameters
 */
template <typename System>
concept IntegratableSystemConcept = SystemConcept<System> && requires(System system, typename System::precision_t h) {
    { system.pre_step(h) };
    { system.post_step(h) };
	// We should check atributes m_variables and m_parameters are present in System,
	// but we cannot do it directly in a concept because they are private members.
    // { System::m_variables } -> std::same_as<typename System::precision_t*>;
    // { System::m_parameters } -> std::same_as<typename System::precision_t*>;
};

#endif /*INTEGRATABLESYSTEMCONCEPT_H_*/
