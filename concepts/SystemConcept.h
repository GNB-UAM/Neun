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

$Id: SystemConcept.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef SYSTEMCONCEPT_H_
#define SYSTEMCONCEPT_H_

#include <concepts>
#include "ModelConcept.h"

/**
 * \class SystemConcept
 *
 * Dynamical systems are a set of variables that evolve with time.
 *
 * A model of this concept must define:
 *
 * The following types
 * \li variable
 * \li parameter
 * \li precision_t
 *
 * The following integers
 * \li n_variables
 * \li n_parameters
 *
 * And the following methods
 * \li precision_t get(variable) const
 * \li precision_t get(parameter) const
 * \li void set(variable, precision_t)
 * \li void set(parameter, precision_t)
 * \li void step(precision_t)
 */
template <typename Wrapee>
concept SystemConcept = ModelConcept<Wrapee> && requires(
  Wrapee wrapee,
  const Wrapee const_wrapee,
  typename Wrapee::variable v,
  typename Wrapee::parameter p,
  typename Wrapee::precision_t value
) {
  { const_wrapee.get(v) } -> std::convertible_to<typename Wrapee::precision_t>;
  { const_wrapee.get(p) } -> std::convertible_to<typename Wrapee::precision_t>;
  { wrapee.set(v, value) };
  { wrapee.set(p, value) };
  { Wrapee::n_variables } -> std::convertible_to<int>;
  { Wrapee::n_parameters } -> std::convertible_to<int>;
};


#endif /*DYNAMICALSYSTEMCONCEPT_H_*/
