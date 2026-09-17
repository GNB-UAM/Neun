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

$Id: IntegratorConcept.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef INTEGRATORCONCEPT_H_
#define INTEGRATORCONCEPT_H_

#include "SystemArchetype.h"
#include <concepts>

/* * \class IntegratorConcept
  * * A model of this concept must meet the requirements for SystemArchetype plus:
  * * The following static methods
  * * \li void step(System &s, precision_t h, precision_t * const variables, precision_t * const parameters)
  * * This method must calculate the next state of the system given the current values of the variables and parameters.
  * * The method must be static, so it can be called without an instance of the Integrator.
  * * The method must take a reference to the system, the time step, and pointers to the variables and parameters.
  * * The method must not return anything, as it modifies the system's state directly.
  */
template <typename Integrator, typename System>
concept IntegratorConcept = 
    requires(
            System& s, 
            typename System::precision_t h,
            typename System::precision_t* const vars, 
            typename System::precision_t* const params) {
      Integrator::template step<System>(s, h, vars, params); 
    };

#endif /*INTEGRATORCONCEPT_H_*/
