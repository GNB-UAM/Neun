/*************************************************************

Copyright (c) 2022 Alicia Garrido Peña
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

$Id: VavoulisCGCModelQ10.h 184 2007-06-04 11:26:12Z elferdo $
*************************************************************/

#ifndef VAVOULISCGCMODELQ10_H_
#define VAVOULISCGCMODELQ10_H_

#include <cmath>
#include "NeuronBase.h"
#include <string>

/**
 * (Hodgkin and Huxley, 1952)
 * cm = 1 * 7.854e-3
 * vna = 50
 * vk = -77
 * vl = -54.387
 * gna = 120 * 7.854e-3
 * gk = 36 * 7.854e-3
 * gl = 0.3 * 7.854e-3
 */

template <typename Precission>
class VavoulisCGCModelQ10 : public NeuronBase<Precission>
{
public:
	typedef Precission precission_t;

	enum variable {v, h, r, a, b, n, e, f, n_variables};
	enum parameter {t_scale, diff_T, cm, gamma_T,
					vna, vk, vca,
					Gnat, Gnap, Ga, Gd, Glva, Ghva,
					Q10_Gnat, Q10_Gnap, Q10_Ga, Q10_Gd, Q10_Glva, Q10_Ghva,
					vh_h, vs_h, tau0_h, delta_h, Q10_h,
					vh_r, vs_r, tau0_r, delta_r, Q10_r,
					vh_a, vs_a, tau0_a, delta_a, Q10_a,
					vh_b, vs_b, tau0_b, delta_b, Q10_b,
					vh_n, vs_n, tau0_n, delta_n, Q10_n,
					vh_e, vs_e, tau0_e, delta_e, Q10_e,
					vh_f, vs_f, tau0_f, delta_f, Q10_f,
					Vh_m, Vs_m,
					Vh_c, Vs_c, Vh_d, Vs_d,
					dv,
					Inat, Inap, Ia, Id, Ilva, Ihva,
					n_parameters};

	static constexpr std::vector<std::string> VarNames()
	{
		return std::vector<std::string> {"v", "h", "r", "a", "b", "n", "e", "f"};
	}

	static constexpr std::vector<std::string> ParamNames()
	{
			return std::vector<std::string> {"t_scale", "diff_T", "cm", "gamma_T",
						"vna", "vk", "vca",
						"Gnat", "Gnap", "Ga", "Gd", "Glva", "Ghva",
						"Q10_Gnat", "Q10_Gnap", "Q10_Ga", "Q10_Gd", "Q10_Glva", "Q10_Ghva",
						"vh_h", "vs_h", "tau0_h", "delta_h", "Q10_h",
						"vh_r", "vs_r", "tau0_r", "delta_r", "Q10_r",
						"vh_a", "vs_a", "tau0_a", "delta_a", "Q10_a",
						"vh_b", "vs_b", "tau0_b", "delta_b", "Q10_b",
						"vh_n", "vs_n", "tau0_n", "delta_n", "Q10_n",
						"vh_e", "vs_e", "tau0_e", "delta_e", "Q10_e",
						"vh_f", "vs_f", "tau0_f", "delta_f", "Q10_f",
						"Vh_m", "Vs_m",
						"Vh_c", "Vs_c", "Vh_d", "Vs_d",
						"dv",
						"Inat", "Inap", "Ia", "Id", "Ilva", "Ihva"
						};
	}
protected:

	Precission x_inf(Precission v, Precission vh, Precission vs) const
	{
		return 1 / (1 + exp((vh - v) / vs));
	}

	Precission incr_x(Precission phi_q10, Precission x, Precission v, Precission vh, Precission vs, Precission tau0, Precission delta) const
	{
		Precission xinf = x_inf(v,vh,vs);

		Precission taux = tau0 * exp(delta * (vh - v) / vs) * xinf;

		return phi_q10 * (xinf - x) / taux;
	}


	//Sodium
	Precission inat(Precission v, Precission h, Precission gnat, Precission vna,
					 Precission vh_m, Precission vs_m) const {
		Precission minf = x_inf(v, vh_m, vs_m);
		return gnat * pow(minf,3) * h * (v - vna);
	}

	Precission inap(Precission v, Precission r, Precission gnap, Precission vna) const {
		return gnap * pow(r,3) * (v - vna);
	}

	//Potassium
	Precission ia(Precission v, Precission a, Precission b, Precission ga, Precission vk) const {
		return ga * pow(a,4) * b * (v - vk);
	}

	Precission id(Precission v, Precission n, Precission gd, Precission vk) const {
		return gd * pow(n,4) * (v - vk);
	}

	//Calcium
	Precission ilva(Precission v, Precission glva, Precission vca, 
					Precission vh_c, Precission vs_c, Precission vh_d, Precission vs_d) const {

		Precission cinf = x_inf(v,vh_c,vs_c);
		Precission dinf = x_inf(v,vh_d,vs_d);
		return glva * pow(cinf,3) * dinf * (v - vca);
	}

	Precission ihva(Precission v, Precission e, Precission f, Precission ghva, Precission vca) const {
		return ghva * pow(e,3) * f *(v - vca);
	}

	Precission g_q10(Precission q10, Precission g, Precission diff_T) const
	{
		if (q10 > 0)
			return g * pow(q10,diff_T/10);
		else
			return 1 * g;
	}

	Precission phi_q10(Precission q10, Precission diff_T) const
	{
		if (q10 > 0)
			return pow(q10,diff_T/10);
		else
			return 1;
	}

	Precission c_q10(Precission cm, Precission gamma_T, Precission diff_T) const
	{
		return cm + cm * gamma_T * diff_T;
	}



public:

  VavoulisCGCModelQ10(ConstructorArgs const &args)
  {
    std::copy(args.params, args.params + n_parameters, m_parameters);
  }

	void eval(const Precission * const vars, Precission * const params, Precission * const incs) const
	{
		incs[h]= params[t_scale] * incr_x(phi_q10(params[Q10_h], params[diff_T]),
					 vars[h], vars[v], params[vh_h], params[vs_h],
					  params[tau0_h], params[delta_h]);

		incs[r]= params[t_scale] * incr_x(phi_q10(params[Q10_r], params[diff_T]),
					 vars[r], vars[v], params[vh_r], params[vs_r],
					  params[tau0_r], params[delta_r]);

		incs[a]= params[t_scale] * incr_x(phi_q10(params[Q10_a], params[diff_T]),
					 vars[a], vars[v], params[vh_a], params[vs_a],
					  params[tau0_a], params[delta_a]);

		incs[b]= params[t_scale] * incr_x(phi_q10(params[Q10_b], params[diff_T]),
					 vars[b], vars[v], params[vh_b], params[vs_b],
					  params[tau0_b], params[delta_b]);

		incs[n]= params[t_scale] * incr_x(phi_q10(params[Q10_n], params[diff_T]),
					 vars[n], vars[v], params[vh_n], params[vs_n],
					  params[tau0_n], params[delta_n]);

		incs[e]= params[t_scale] * incr_x(phi_q10(params[Q10_e], params[diff_T]),
					 vars[e], vars[v], params[vh_e], params[vs_e],
					  params[tau0_e], params[delta_e]);

		incs[f]= params[t_scale] * incr_x(phi_q10(params[Q10_f], params[diff_T]),
					 vars[f], vars[v], params[vh_f], params[vs_f],
					  params[tau0_f], params[delta_f]);


		params[Inat] = inat(vars[v], vars[h], g_q10(params[Q10_Gnat], params[Gnat], params[diff_T]), params[vna],
									 params[Vh_m], params[Vs_m]);
		params[Inap] = inap(vars[v], vars[r], g_q10(params[Q10_Gnap], params[Gnap], params[diff_T]), params[vna]);
		params[Ia] = ia(vars[v], vars[a], vars[b], g_q10(params[Q10_Ga], params[Ga], params[diff_T]), params[vk]);
		params[Id] = id(vars[v], vars[n], g_q10(params[Q10_Gd], params[Gd], params[diff_T]), params[vk]);
		params[Ilva] = ilva(vars[v], g_q10(params[Q10_Glva], params[Glva], params[diff_T]), params[vca],
								 params[Vh_c], params[Vs_c], params[Vh_d], params[Vs_d]);
		params[Ihva] = ihva(vars[v], vars[e], vars[f], g_q10(params[Q10_Ghva], params[Ghva], params[diff_T]), params[vca]);

		incs[v] = params[t_scale] * (SYNAPTIC_INPUT 
					- params[Inat] - params[Inap] 
					- params[Ia] - params[Id]
					- params[Ilva] - params[Ihva])
					/ c_q10(params[cm], params[gamma_T], params[diff_T]);

		params[dv] = incs[v];

		// params[Inat] = inat(vars[v], vars[h], Q10G(params[Gnat]), params[vna], params[Vh_m], params[Vs_m]);
		// params[Inap] = inap(vars[v], vars[r], params[Gnap], params[vna]);
		// params[Ia] = ia(vars[v], vars[a], vars[b], params[Ga], params[vk]);
		// params[Id] = id(vars[v], vars[n], params[Gd], params[vk]);
		// params[Ilva] = ilva(vars[v], params[Glva], params[vca], params[Vh_c], params[Vs_c], params[Vh_d], params[Vs_d]);
		// params[Ihva] = ihva(vars[v], vars[e], vars[f], params[Ghva], params[vca]);

	}

};

#endif /*VAVOULISCGCMODELQ10_H_*/
