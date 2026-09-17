/*************************************************************

Copyright (c) 2020, Ángel Lareo Fernández
                                        Alicia Garrido Peña
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

#ifndef VAVOULISMODEL_H_
#define VAVOULISMODEL_H_

#include <cmath>
#include "NeuronBase.h"

template <typename Precision>
class VavoulisModel : public NeuronBase<Precision> {
 public:
  typedef Precision precision_t;

  // clang-format off
  enum variable { v, va, p, q, h, n, n_variables };
  enum parameter { n_type, tau_p, tau_q, g_eca, g_ecs, n_parameters };
  enum type {so, n1m, n2v, n3t, n_types};
  // clang-format on

 protected:
  Precision il(Precision v) const { return v + 67; }

  Precision ix(type t, Precision v, Precision p, Precision q) const {
    switch (t) {
      case so:
        return 0;
        break;
      case n1m:
        return 200 * p * p * p * (v + 30);
        break;
      case n2v:
        return 2 * p * p * p * q * (v - 55);
        break;
      case n3t:
        return 3.27 * p * p * p * q * (v - 80);
        break;
      default:
        return 0;
         break;
    }
  }

  // iecs(v,va,g_ecs) - ieca(va,v,g_eca)
  Precision iec(Precision v1, Precision v2, Precision g_ec) const {
    return g_ec * (v1 - v2);
  }

  Precision inat(Precision va, Precision h) const {
    Precision minf = 1 / (1 + exp((-34.6 - va) / 9.6));
    return 350 * minf * minf * minf * h * (va - 55);
  }

  Precision ik(Precision va, Precision n) const {
    return 90 * n * n * n * n * (va + 90);
  }

  Precision incr_p(type t, Precision p, Precision v,
                    Precision tau_p) const {
    // switch (t) {
    Precision pinf = 1 / (1 + exp((-61.6 - v) / 5.6));
    return (pinf - p) / tau_p;
  }

  Precision incr_q(type t, Precision q, Precision v,
                    Precision tau_q) const {
    // switch (t) {
    Precision qinf = 1 / (1 + exp((-73.2 - v) / -5.1));
    return (qinf - q) / tau_q;
  }

  Precision incr_h(Precision h, Precision va) const {
    Precision tau_h =
        1.1 + 7.2 * exp(-(pow(((-61.3 - va)/22.7),2)));
    Precision hinf = 1 / (1 + exp((-55.2 - va) / -7.1));
    return (hinf - h) / tau_h;
  }

  Precision incr_n(Precision n, Precision va) const {
    Precision tau_n =
        1.1 + 4.6 * exp(-(pow(((-61 - va)/54.3),2)));
    Precision ninf = 1 / (1 + exp((-30 - va) / 17.4));
    return (ninf - n) / tau_n;
  }

 public:
  VavoulisModel(ConstructorArgs const &args) {
    std::copy(args.params, args.params + n_parameters, m_parameters);
    std::copy(args.variables, args.variables + n_variables, m_variables);
  }

  void eval(const Precision *const vars, Precision *const params,
            Precision *const incs) const {
    incs[v] = (-SYNAPTIC_INPUT - il(vars[v]) -
                  ix((type)params[n_type], vars[v], vars[p], vars[q]) -
                  iec(vars[v], vars[va], vars[g_ecs]))/10;

    incs[va] = (-il(vars[va]) - inat(vars[va], vars[h]) - ik(vars[va], vars[n]) -
                   iec(vars[va], vars[v], vars[g_eca]))/10;

    incs[p] = incr_p((type)params[n_type], vars[p], vars[v], params[tau_p]);
    incs[q] = incr_q((type)params[n_type], vars[q], vars[v], params[tau_q]);

    incs[h] = incr_h(vars[h], vars[va]);
    incs[n] = incr_n(vars[n], vars[va]);
  }
};

#endif /*VAVOULISMODEL_H_*/
