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

#include <DifferentialNeuronWrapper.h>
#include <STDPSynapse.h>
#include <HodgkinHuxleyModel.h>
#include <SystemWrapper.h>
#include <RungeKutta4.h>
#include <iostream>
#include <cstdlib>

typedef RungeKutta4 Integrator;
typedef DifferentialNeuronWrapper<SystemWrapper<HodgkinHuxleyModel<double>>, Integrator> HH;
typedef STDPSynapse<HH, HH, Integrator, double> Synapse;

int main(int argc, char **argv) {

  // Struct to initialize neuron model parameters
  HH::ConstructorArgs args;

  // Set the parameter values
  args.params[HH::cm] = 1 * 7.854e-3;
  args.params[HH::vna] = 50;
  args.params[HH::vk] = -77;
  args.params[HH::vl] = -54.387;
  args.params[HH::gna] = 120 * 7.854e-3;
  args.params[HH::gk] = 36 * 7.854e-3;
  args.params[HH::gl] = 0.3 * 7.854e-3;

  // Struct to initialize synapse model parameters
  Synapse::ConstructorArgs syn_args;
  syn_args.params[Synapse::A_minus] = 0.00525;
  syn_args.params[Synapse::A_plus] = 0.005;
  syn_args.params[Synapse::tau_minus] = 20;
  syn_args.params[Synapse::tau_plus] = 20;
  syn_args.params[Synapse::spike_threshold] = -54;
  syn_args.params[Synapse::g_max] = 1;
  syn_args.params[Synapse::g_min] = 0;
  syn_args.params[Synapse::E_syn] = 0;
  syn_args.params[Synapse::tau_syn] = 5;

  // Set the integration step
  const double step = 0.005;
  double simulation_time = 10000;
  // double simulation_time = 200;

  // Initialize neuron models
  HH h1(args), h3(args), h2(args);

  // Set initial value of V
  h1.set(HH::v, -75);
  h3.set(HH::v, -85);
  h2.set(HH::v, -70);
  
  // Initialize synapse
  Synapse s1(h1, HH::v, h2, HH::v, syn_args, 1);
  Synapse s2(h3, HH::v, h2, HH::v, syn_args, 1);

  // Initialize g
  s1.set_g(0.0052);
  s2.set_g(0.005);

  std::cout << "Time vpre1 vpre2 vpost i1 i2 g1 g2" << std::endl;

  int slower = 0;
  for (double time = 0; time < simulation_time; time += step) {
    s1.step(step, h1.get(HH::v), h2.get(HH::v));
    s2.step(step, h3.get(HH::v), h2.get(HH::v));

    h1.add_synaptic_input(0.6);
    h3.add_synaptic_input(0.5);

    h2.add_synaptic_input(0.5);
    h2.add_synaptic_input(s1.get(Synapse::i));
    h2.add_synaptic_input(s2.get(Synapse::i));
    
    h1.step(step);
    h3.step(step);
    h2.step(step);

    std::cout << time << " " << h1.get(HH::v) << " " << h3.get(HH::v) << " " << h2.get(HH::v) << " " 
              << s1.get(Synapse::i) << " " << s2.get(Synapse::i) << " " 
              << s1.get(Synapse::g) << " " << s2.get(Synapse::g) << "\n";
  }

  return 0;
}
