/*************************************************************

Copyright (c) 2006, Fernando Herrero Carrón
              2013-2020, Ángel Lareo <angel.lareo@gmail.com>
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

$Id: ElectricalSynapse.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef ELECTRICALSYNAPSE_H_
#define ELECTRICALSYNAPSE_H_

#include <iostream>
#include <type_traits>

#include "NeuronConcept.h"

/**
 * @brief Implements a synapse that balances current between two neurons
 * depending on the difference of their potentials.
 * @param TNode1 Type of the first neuron
 * @param TNode2 Type of the second neuron
 */

template <typename TNode1, typename TNode2, typename precision = double>
requires NeuronConcept<TNode1> && NeuronConcept<TNode2>
class ElectricalSynapse {
  static_assert(std::is_floating_point<precision>::value);

 public:
  enum variable { i1, i2, n_variables };
  enum parameter { g1, g2, n_parameters };

  typedef precision precision_t;

 private:
  ElectricalSynapse(ElectricalSynapse &s) {}

  void operator=(ElectricalSynapse &s) {}

  TNode1 &m_n1;
  TNode2 &m_n2;

  const typename TNode1::variable m_n1_variable;
  const typename TNode2::variable m_n2_variable;

  precision m_variables[n_variables];
  precision m_parameters[n_parameters];

 public:
  /**
   * @param n1 The first neuron
   * @param v1 An enum value representing the variables from which to measure
   *           the difference of potential
   * @param n2 The second neuron
   * @param v2 Similar to @c v1
   * @param pg1 Synaptic weight from @c n2 to n1
   * @param pg2 Synaptic weight from @c n1 to n2
   */
  ElectricalSynapse(TNode1 &n1, typename TNode1::variable v1, TNode2 &n2,
                     typename TNode2::variable v2, precision pg1,
                     precision pg2)
      : m_n1(n1), m_n2(n2), m_n1_variable(v1), m_n2_variable(v2) {
    m_parameters[g1] = pg1;
    m_parameters[g2] = pg2;
  }

  void step(precision h) {
    m_variables[i1] =
        m_parameters[g1] * (m_n2.get(m_n2_variable) - m_n1.get(m_n1_variable));
    m_variables[i2] =
        m_parameters[g2] * (m_n1.get(m_n1_variable) - m_n2.get(m_n2_variable));

    m_n1.add_synaptic_input(m_variables[i1]);
    m_n2.add_synaptic_input(m_variables[i2]);
  }

  precision_t get(variable var) const { return m_variables[var]; }

  void set(variable var, precision_t value) { m_variables[var] = value; }

  precision_t get(parameter param) const { return m_parameters[param]; }

  void set(parameter param, precision_t value) { m_parameters[param] = value; }

  void save(std::ostream &os) {
    for (int i = 0; i < n_variables; i++) {
      os << m_variables[i] << " ";
    }
  }

  void load(std::istream &is) {
    for (int i = 0; i < n_variables; i++) {
      is >> m_variables[i];
    }
  }
};

#endif /*ELECTRICALSYNAPSE_H_*/
