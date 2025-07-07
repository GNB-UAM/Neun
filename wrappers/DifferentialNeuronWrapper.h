/*************************************************************

Copyright (c) 2006, Fernando Herrero Carrón
              2020, Angel Lareo <angel.lareo@gmail.com>
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

$Id$
*************************************************************/
#ifndef DIFFERENTIALNEURONWRAPPER_H_
#define DIFFERENTIALNEURONWRAPPER_H_


#include <type_traits>

#include "NeuronConcept.h"
#include "DynamicalSystemWrapper.h"
#include "DynamicalSystemConcept.h"
#include "IntegratorConcept.h"

/**
 * \brief Adds common code to a model class.
 * \todo Check concepts
 * \param Wrapee The model class to extend.
 * \param Integrator The integrator class to use
 */

template <typename Wrapee, typename Integrator>
requires NeuronConcept<Wrapee>
class DifferentialNeuronWrapper : public DynamicalSystemWrapper<Wrapee> {
  static_assert(std::is_floating_point<typename Wrapee::precission_t>::value,
                "Wrapee must have a floating point precission_t type");

  static_assert(DynamicalSystemConcept<Wrapee>, "Wrapee must satisfy DynamicalSystemConcept");

  static_assert(IntegratorConcept<Integrator, Wrapee>,
                "Integrator must satisfy IntegratorConcept");

 public:
  typedef typename Wrapee::precission_t precission_t;
  typedef typename Wrapee::variable variable;
  typedef typename Wrapee::parameter parameter;
  typedef typename Wrapee::ConstructorArgs ConstructorArgs;

  DifferentialNeuronWrapper(ConstructorArgs &args)
      : DynamicalSystemWrapper<Wrapee>(args) {}

  void step(precission_t h) {
    Integrator::step(*this, h, Wrapee::m_variables, Wrapee::m_parameters);

    Wrapee::m_synaptic_input = 0;
  }

  void add_synaptic_input(precission_t i) { Wrapee::m_synaptic_input += i; }

  precission_t get_synaptic_input() const { return Wrapee::m_synaptic_input; }
};

#endif /*DIFFERENTIALNEURONWRAPPER_H_*/
