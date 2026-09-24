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

#ifndef ROWATSELVERSTONMODEL_H_
#define ROWATSELVERSTONMODEL_H_

#include <algorithm>
#include <cmath>
#include <type_traits>

#include "NeuronBase.h"

/** (Rowat and Selverston, 1997)
 * tm = 1
 * sf = 2
 * ts = 2
 * ss = 1
 * af = 1
 * es = 1
 */
template <typename Precision>
class RowatSelverstonModel : public NeuronBase<Precision> {
  static_assert(std::is_floating_point<Precision>::value);

 public:
  typedef Precision precision_t;

  enum variable { v = 0, q = 1, n_variables };
  enum parameter { tm, sf, ts, ss, af, es, n_parameters };

 public:
  void eval(const Precision *const vars, Precision *const params, Precision *const incs) const {
    incs[v] = -(vars[v] -
                params[af] *
                    tanh((params[sf] / params[af]) * vars[v]) +
                vars[q] - SYNAPTIC_INPUT) /
              params[tm];
    incs[q] = (-vars[q] + params[ss] * (vars[v] - params[es])) /
              params[ts];
  }
};

#endif /*ROWATSELVERSTONMODEL_H_*/
