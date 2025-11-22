/*************************************************************

Copyright (c) 2025, Alicia Garrido Peña <alicia.garrido@uam.es>
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
#include <ChemicalSynapsis.h>
#include <HodgkinHuxleyModel.h>
#include <SystemWrapper.h>
#include <RungeKutta4.h>
#include <iostream>

typedef RungeKutta4 Integrator;
typedef DifferentialNeuronWrapper<SystemWrapper<HodgkinHuxleyModel<double>>, Integrator> HH;
typedef ChemicalSynapsis<HH, HH, Integrator, double> Synapsis;
// typedef ChemicalSynapsisModel<double> SynapsisModel;

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


  Synapsis::ConstructorArgs syn_args;
  syn_args.params[Synapsis::gfast] = 0.015;
  syn_args.params[Synapsis::Esyn] = -75;
  syn_args.params[Synapsis::sfast] = 0.2;
  syn_args.params[Synapsis::Vfast] = -50;
  syn_args.params[Synapsis::gslow] = 0.025; //When 0, use only fast
  syn_args.params[Synapsis::k1] = 1;
  syn_args.params[Synapsis::k2] = 0.03;
  syn_args.params[Synapsis::sslow] = 1;


  // Initialize neuron models
  HH h1(args), h2(args);

  // Set initial value of V in neuron n1
  h1.set(HH::v, -75);

  // Set the integration step
  const double step = 0.01;

  // Initialize a synapsis between the neurons
  Synapsis s(h1, HH::v, h2, HH::v, syn_args, 1);


  // Perform the simulation
  double simulation_time = 1000;
  std::cout << "Time" << " " << "Vpre" << " " << "Vpost" 
              << " " << "i" << " " << "ifast" << " " << "islow"
              << std::endl;

  for (double time = 0; time < simulation_time; time += step) {
    s.step(step, h1.get(HH::v), h2.get(HH::v));

    // Provide an external current input to both neurons
    h1.add_synaptic_input(0.5);
    h2.add_synaptic_input(0.5);

    h2.add_synaptic_input(s.get(Synapsis::i));

    h1.step(step);
    h2.step(step);

    std::cout << time << " " << h1.get(HH::v) << " " << h2.get(HH::v) 
              << " " << s.get(Synapsis::i)<< " " << s.get(Synapsis::ifast) << " " << s.get(Synapsis::islow)
              << std::endl;
  }

  return 0;
}
