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

#ifndef STDPSYNAPSEMODEL_H_
#define STDPSYNAPSEMODEL_H_

/**
* @brief Implements a synapse based on (Song, Miller & Abbott, 2000)
*/
template <typename precission = double>
class STDPSynapseModel {
#ifndef __AVR_ARCH__
  static_assert(std::is_floating_point<precission>::value);
#endif  //__AVR_ARCH__

  public:
    enum variable {
      g,              // Synaptic conductance (w)
      s,              // Synaptic gating variable
      n_variables
    };

    enum parameter {
      v_pre,        // Presynaptic neuron's voltage (membrane potential)
      v_post,       // Postsynaptic neuron's voltage (membrane potential)
      A_minus,      // Amplitude of synaptic modification (for LTD) (sugested 0.00525) (A- / A+ = 1.05)
      A_plus,       // Amplitude of synaptic modification (for LTP) (sugested 0.005)
      tau_minus,    // Time constant for decrease in synaptic weight (sugested 20ms)
      tau_plus,     // Time constant for increase in synaptic weight (sugested 20ms)
      g_max,        // Maximum allowed synaptic conductance
      g_min,        // Minimum allowed synaptic conductance
      i,            // Synaptic intensity
      E_syn,        // Reversal potential
      tau_syn,      // Time constant
      spike_threshold,
      n_parameters
    };
    
    typedef precission precission_t;

  public:
    STDPSynapseModel() {}

    void eval(const precission* const vars, const precission* const params,
              precission* const incs) const {

      incs[g] = 0; // d(g)/dt = 0

      incs[s] = -vars[s] / params[tau_syn]; // d(s)/dt = -s/tau_syn
      if(vars[s] < 0) incs[s] = 0;
    }
  
};

#endif /*STDPSYNAPSEMODEL_H_*/
