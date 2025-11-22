/*************************************************************

Copyright (c) 2025  Alicia Garrido-Peña <alicia.garrido@uam.es>
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

#ifndef CHEMICAL_SYNAPSIS_MODEL_H_
#define CHEMICAL_SYNAPSIS_MODEL_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#endif  //__AVR_ARCH__
#include<cmath>

/**
 * @brief Implements a synapsis based on  (Golowasch et al., 1999 )
 */
template <typename precission = double>
class ChemicalSynapsisModel {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precission>::value);
#endif  //__AVR_ARCH__

 public:
  enum variable {mslow, n_variables};
  enum parameter {
    v_pre,
    gfast,
    Esyn,
    sfast,
    Vfast,
    gslow,
    k1,
    k2,
    sslow,
    Vslow,
    ifast,
    islow,
    i,
    n_parameters
  };
  
  static constexpr std::vector<std::string> ParamNames()
    {
        return std::vector<std::string> {
            "v_pre",
            "gfast",
            "Esyn",
            "sfast",
            "Vfast",
            "gslow",
            "k1",
            "k2",
            "sslow",
            "Vslow",
            "ifast",
            "islow",
            "i",
            "n_parameters"
              };
    }

  typedef precission precission_t;

 public:
  ChemicalSynapsisModel() {}

  void eval(const precission* const vars, const precission* const params,
            precission* const incs) const {
      incs[mslow] = ((params[k1] * (1 - params[mslow])) / (1 + exp(params[sslow] * (params[Vslow] - params[v_pre])))) - params[k2] * vars[mslow];
  }
};

#endif /*CHEMICAL_SYNAPSIS_MODEL_H_*/
