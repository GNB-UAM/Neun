/*************************************************************

Copyright (c) 2016, Ángel Lareo Fernández
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

$Id: HindmarshRoseModel.h 203 2007-06-26 14:46:34Z elferdo $
*************************************************************/

#ifndef IZHIKEVICHMODEL_H_
#define IZHIKEVICHMODEL_H_

#include <algorithm>
#include <cmath>

#include "NeuronBase.h"

/**
 * (Izhikevich, 2006)
 */

template <typename Precision>
class IzhikevichModel : public NeuronBase<Precision> {
 public:
  typedef Precision precision_t;

  enum variable { v, u, n_variables };
  enum parameter { a, b, c, d, threshold, n_parameters };

 public:
  void eval(const Precision *const vars, Precision *const params,
            Precision *const incs) const {
    incs[v] =
        0.04 * vars[v] * vars[v] + 5 * vars[v] + 140 - vars[u] + SYNAPTIC_INPUT;
    // incs[v] = v + 0.5*(0.04 * vars[v] * vars[v] + 5 * vars[v] + 140 - vars[u]
    // + SYNAPTIC_INPUT);  incs[v] = v + 0.5*(0.04 * vars[v] * vars[v] + 5 *
    // vars[v] + 140 - vars[u] + SYNAPTIC_INPUT);
    incs[u] = params[a] * (params[b] * vars[v] - vars[u]);
  }

  void post_step(Precision h) {
    if (this->get(v) >= this->get(threshold)) {
      this->set(v, this->get(c));
      this->set(u, this->get(u) + this->get(d));
    }
  }
};

#endif /*IZHIKEVICHMODEL_H_*/
