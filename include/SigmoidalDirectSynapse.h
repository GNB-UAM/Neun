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

$Id: SigmoidalDirectSynapse.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef SIGMOIDALDIRECTSYNAPSE_H_
#define SIGMOIDALDIRECTSYNAPSE_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#include <concepts>

#include "../concepts/NeuronConcept.h"
#endif  //__AVR_ARCH__

/**
 * Implements a sigmoidal synapse
 */

template <typename TNode1, typename TNode2, typename precission = double>
requires NeuronConcept<TNode1> && NeuronConcept<TNode2>
class SigmoidalDirectSynapse {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precission>::value);
#endif  //__AVR_ARCH__

 public:
  enum parameter { g, t, sigma, shift, n_parameters };

 private:
  void operator=(SigmoidalDirectSynapse &s) {}

  precission m_parameters[n_parameters];

  TNode1 const &m_n1;
  TNode2 &m_n2;

  const typename TNode1::variable m_n1_variable;

 public:
  SigmoidalDirectSynapse(TNode1 const &n1, typename TNode1::variable v,
                          TNode2 &n2, precission pg = 1, precission pt = 0,
                          precission psigma = 1, precission pshift = 0)
      : m_n1(n1), m_n2(n2), m_n1_variable(v) {
    m_parameters[g] = pg;
    m_parameters[t] = pt;
    m_parameters[sigma] = psigma;
    m_parameters[shift] = pshift;
  }

  SigmoidalDirectSynapse(TNode1 const &n1, TNode2 &n2,
                          SigmoidalDirectSynapse const &s)
      : m_n1(n1), m_n2(n2), m_n1_variable(s.m_n1_variable) {
    m_parameters[g] = s.m_parameters[g];
    m_parameters[t] = s.m_parameters[t];
    m_parameters[sigma] = s.m_parameters[sigma];
    m_parameters[shift] = s.m_parameters[shift];
  }

  void step(precission h) {
    //		const precission value = m_parameters[g] * (m_parameters[shift]
    //+ tanh(m_parameters[sigma] * m_n1.get_variable(m_n1_variable) -
    // m_parameters[t]));

    const precission value = m_n1.get(m_n1_variable);

    if (value > m_parameters[t]) {
      m_n2.add_synaptic_input(m_parameters[g]);
    }
  }
};

#endif /*SIGMOIDALDIRECTSYNAPSE_H_*/
