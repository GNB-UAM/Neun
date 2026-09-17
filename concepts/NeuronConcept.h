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

$Id: NeuronConcept.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef NEURONCONCEPT_H_
#define NEURONCONCEPT_H_

#include <concepts>
#include "SystemConcept.h"

/*
 * \class NeuronConcept
 *
 * A neuron is a system that has a synaptic input and can add it to its state.
 *
 * A model of this concept must meet the requirements for SystemConcept plus:
 *
 * The following methods
 * \li void add_synaptic_input(precision_t value)
 * \li precision_t get_synaptic_input() const
 * \li void reset_synaptic_input()
 */
template <typename Neuron>
concept NeuronConcept = SystemConcept<Neuron> && 
  requires(Neuron model, const Neuron const_model, typename Neuron::precision_t value) {
    { model.add_synaptic_input(value) };
    { const_model.get_synaptic_input() } -> std::convertible_to<typename Neuron::precision_t>;
    { model.reset_synaptic_input() };
    // pre_step and post_step are optional for neurons
    { model.pre_step(value) } -> std::same_as<void>;
    { model.post_step(value) } -> std::same_as<void>;
  };

#endif /*NEURONCONCEPT_H_*/
