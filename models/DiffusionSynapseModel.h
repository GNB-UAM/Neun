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

*************************************************************/

#ifndef DIFFUSIONSYNAPSEMODEL_H_
#define DIFFUSIONSYNAPSEMODEL_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#endif  //__AVR_ARCH__

/**
 * @brief Implements a synapse based on (Destexhe et al. 1994)
 */
template <typename precision = double>
class DiffusionSynapseModel {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precision>::value);
#endif  //__AVR_ARCH__

 public:
  enum variable { r, i, n_variables };
  enum parameter {
    alpha,
    beta,
    threshold,
    esyn,
    gsyn,
    T,
    max_release_time,
    n_parameters
  };

  typedef precision precision_t;

 protected:
  bool m_release;

 public:
  DiffusionSynapseModel() : m_release(false) {}

  void eval(const precision* const vars, const precision* const params,
            precision* const incs) const {
    if (m_release) {
      incs[r] =
          params[alpha] * params[T] * (1 - vars[r]) - params[beta] * vars[r];
    } else {
      incs[r] = -params[beta] * vars[r];
    }
  }
};

#endif /*DIFFUSIONSYNAPSEMODEL_H_*/
