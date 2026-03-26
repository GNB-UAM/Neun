/*************************************************************

Copyright (c) 2021, Angel Lareo <angel.lareo@gmail.com>
                    Alicia Garrido-Peña <alicia.garrido@uam.es>
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

#ifndef GRADUAL_ACTIVATION_SYNAPSIS_MODEL_H_
#define GRADUAL_ACTIVATION_SYNAPSIS_MODEL_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#endif  //__AVR_ARCH__

/**
 * @brief Implements a synapsis based on (Destexhe et al. 1994)
 */
template <typename precission = double>
class GradualActivationSynapsisModel {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precission>::value);
#endif  //__AVR_ARCH__

 public:
  enum variable { r, s, i, n_variables };
  enum parameter {
    esyn,
    gsyn,
    tau_syn,
    v_pre,
    v_r,
    dec_slope,
    n_parameters
  };

  typedef precission precission_t;

 public:
  GradualActivationSynapsisModel() {}

  void eval(const precission* const vars, const precission* const params,
            precission* const incs) const {
    
      precission r_inf = 1 / (1 + exp( (params[v_r] - params[v_pre]) / params[dec_slope]));
      incs[r] = (r_inf - vars[r]) / params[tau_syn];
      incs[s] = (vars[r] - vars[s]) / params[tau_syn];
  }
};

#endif /*GRADUAL_ACTIVATION_SYNAPSIS_MODEL_H_*/
