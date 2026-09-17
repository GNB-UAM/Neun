/*************************************************************

Copyright (c) 2026  Gonzalo Jiménez Carretero
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

*************************************************************/

#ifndef LINSKER_SYNAPSE_MODEL_H_
#define LINSKER_SYNAPSE_MODEL_H_

#include "SynapseWeightNormalizer.h"

/**
 * @brief Implements a synapsis based on (Linsker 1986)
 */
template <typename precission = double>
class LinskerSynapseModel {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precission>::value);
#endif  //__AVR_ARCH__

 public:
    enum variable {
      w,        // Synaptic efficacy
      n_variables
    };

    enum parameter {
      v_pre,    // Presynaptic neuron's voltage (membrane potential)
      v_post,   // Postsynaptic neuron's voltage (membrane potential)
      xo,       // Constant for presynaptic neuron
      yo,       // Constant for postsynaptic neuron
      eta,      // Learning rate
      k1,       // Constant
      w_max,    // Maximum allowed synaptic weight (w_min = -wmax)
      i,        // Synaptic intensity
      n_parameters
    };

    typedef precission precission_t;

 public:
    LinskerSynapseModel() {}

    void eval(const precission* const vars, const precission* const params,
              precission* const incs) const {
        incs[w] = params[eta] * ((params[v_pre] - params[xo]) * (params[v_post] - params[yo]) + params[k1]);
    }
};

#endif /*LINSKER_SYNAPSE_MODEL_H_*/
