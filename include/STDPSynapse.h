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

#ifndef STDPSYNAPSE_H_
#define STDPSYNAPSE_H_

#ifndef __AVR_ARCH__
#include <type_traits>
#include <concepts>
#include "IntegratorConcept.h"
#include "NeuronConcept.h"
#endif //__AVR_ARCH__

#include "STDPSynapseModel.h"
#include "IntegratedSystemWrapper.h"
#include "SerializableWrapper.h"
#include "SystemWrapper.h"
#include <cmath>

#define CURRENT_DIRECTION -1

/**
* Implements a synapse based on (Song, Miller & Abbott, 2000)
*/

template <typename TNode1, typename TNode2, typename TIntegrator, typename precission = double>
requires NeuronConcept<TNode1> && NeuronConcept<TNode2> &&
    IntegratorConcept<TIntegrator, SerializableWrapper<
          SystemWrapper<STDPSynapseModel<precission> > > >

class STDPSynapse : public SerializableWrapper<
          SystemWrapper<STDPSynapseModel<precission> > > {
 private:
  #ifndef __AVR_ARCH__
    static_assert(std::is_floating_point<precission>::value);
  #endif  //__AVR_ARCH__

  precission m_vpre_old;
  precission m_vpost_old;
  precission m_current_time;
  precission m_last_spike_pre;
  precission m_last_spike_post;
  TNode1 const &m_n1;
  TNode2 &m_n2;


  const typename TNode1::variable m_n1_variable;
  const typename TNode2::variable m_n2_variable;


  typedef SerializableWrapper<
      SystemWrapper<STDPSynapseModel<precission> > > System;

  const int m_steps;

 public:
  typedef typename System::precission_t precission_t;
  typedef typename System::variable variable;
  typedef typename System::parameter parameter;
  typedef typename System::ConstructorArgs ConstructorArgs;

  STDPSynapse (TNode1 const &n1, typename TNode1::variable v1,
                                              TNode2 &n2, typename TNode2::variable v2, 
                                              ConstructorArgs &args, int steps)
      : m_n1(n1),
        m_n2(n2),
        m_n1_variable(v1),
        m_n2_variable(v2),
        System(args),
        m_steps(steps) {
          for(int i = 0; i < System::n_variables; i++) {
            System::m_variables[i] = 0;
          }
          m_vpre_old = System::m_parameters[System::spike_threshold] -999;
          m_vpost_old = System::m_parameters[System::spike_threshold] -999;
          m_current_time = 0.0;
          m_last_spike_pre = -999;
          m_last_spike_post = -999;
        }

  STDPSynapse (TNode1 const &n1, TNode2 &n2,
                            STDPSynapse  const &synapse)
      : m_n1(n1),
        m_n2(n2),
        m_n1_variable(synapse.m_n1_variable),
        m_n2_variable(synapse.m_n2_variable),
        m_steps(synapse.m_steps),
        System(synapse) {
          for(int i = 0; i < System::n_variables; i++) {
            System::m_variables[i] = 0;
          }
          m_vpre_old = System::m_parameters[System::spike_threshold] -999;
          m_vpost_old = System::m_parameters[System::spike_threshold] -999;
          m_current_time = 0.0;
          m_last_spike_pre = -999;
          m_last_spike_post = -999;
        }
 private:
  void calculate_i() {
    precission E_syn = System::m_parameters[System::E_syn];

  // Isyn = gsyn * s * (V - Esyn)

  System::m_parameters[System::i] = CURRENT_DIRECTION * System::m_variables[System::g] * System::m_variables[System::s] * (System::m_parameters[System::v_post] - E_syn);
  }

  void update_g(precission h) {
    
    // Update internal simulation clock
    m_current_time += h;
    precission threshold = System::m_parameters[System::spike_threshold];

    // Detect spikes // ----------------------------------------------------------------------------------
     
    // For vpre
    if (System::m_parameters[System::v_pre] >= threshold && m_vpre_old < threshold) {
      m_last_spike_pre = m_current_time;
      System::m_variables[System::s] = 1.0; // Update s if vpre spikes
      
      // △t = t_pre - t_post
      precission delta_t = m_last_spike_pre - m_last_spike_post; 

      // LTD {F(△t) = (-A- * exp(-△t/ τ-)) if △t > 0}
      if (delta_t > 0) {
        precission f_delta_t = (-System::m_parameters[System::A_minus] * std::exp(-delta_t / System::m_parameters[System::tau_minus]));
        System::m_variables[System::g] += f_delta_t; // g := g + F(△t)
      }
    }

    // For vpost
    if (System::m_parameters[System::v_post] >= threshold && m_vpost_old < threshold) {
      m_last_spike_post = m_current_time;
      
      // △t = t_pre - t_post
      precission delta_t = m_last_spike_pre - m_last_spike_post; 

      // LTP {F(△t) = (A+ * exp(△t/ τ+)) if △t < 0}
      if (delta_t < 0) {
        precission f_delta_t = (System::m_parameters[System::A_plus] * std::exp(delta_t / System::m_parameters[System::tau_plus]));
        System::m_variables[System::g] += f_delta_t; // g := g + F(△t)
      }
    }
    // ---------------------------------------------------------------------------------------------------

    /* Limit growth */
    if (System::m_variables[System::g] > System::m_parameters[System::g_max]) {
      System::m_variables[System::g] = System::m_parameters[System::g_max];
    }

    /* Limit decay */
    if (System::m_variables[System::g] < System::m_parameters[System::g_min]) {
      System::m_variables[System::g] = System::m_parameters[System::g_min];
    }

    // Save previous voltages for the next integration step
    m_vpre_old = System::m_parameters[System::v_pre];
    m_vpost_old = System::m_parameters[System::v_post];
  }

 public:
  void step(precission h) {
    
    precission v_pre = m_n1.get(m_n1_variable);
    System::m_parameters[System::v_pre] = v_pre;
    precission v_post = m_n2.get(m_n2_variable);
    System::m_parameters[System::v_post] = v_post;

    for (int i = 0; i < m_steps; ++i) {
      TIntegrator::step(*this, h, System::m_variables, System::m_parameters);
    }

    update_g(h);

    // Calculate synaptic current
    calculate_i();

  }

  void step(precission h, precission vpre, precission vpost) {
    
    System::m_parameters[System::v_pre] = vpre;
    System::m_parameters[System::v_post] = vpost;

    for (int i = 0; i < m_steps; ++i) {
      TIntegrator::step(*this, h, System::m_variables, System::m_parameters);
    }

    update_g(h);

    // Calculate synaptic current
    calculate_i();
  }

  void set_g(precission g) {
    System::m_variables[System::g] = g;
  }

};

#endif /*STDPSYNAPSE_H_*/
