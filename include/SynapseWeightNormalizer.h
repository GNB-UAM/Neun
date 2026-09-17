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


#ifndef SYNAPSEWEIGHTNORMALIZER_H_
#define SYNAPSEWEIGHTNORMALIZER_H_

#include "NormalizableSynapseConcept.h"
#include "NeuronConcept.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>


/**
 * @brief Implements a Singleton Mediator to normalize synapse weights
 */
template <typename Neuron, typename Synapse> // Neuron's type, synapse's type
requires NeuronConcept<Neuron>
class SynapseWeightNormalizer {

 public:

  using precission = typename Synapse::precission_t;
  static SynapseWeightNormalizer& get_instance() {
    static SynapseWeightNormalizer instance;
    return instance;
  }

  // Deny copying or reassigning the constructor (Singleton)
  SynapseWeightNormalizer(const SynapseWeightNormalizer&) = delete;
  void operator=(const SynapseWeightNormalizer&) = delete;


  /**
   * @brief Adds a synapse to the map
   */
  void add_synapse(Synapse* synapse, Neuron* post_synaptic_neuron)  requires NormalizableSynapseConcept<Synapse>{
    neuron_synapses_map[post_synaptic_neuron].push_back(synapse);
  }

  /**
   * @brief Removes a synapse from the map
   */
  void remove_synapse(Synapse* synapse, Neuron* post_synaptic_neuron) {
    auto map_iterator = neuron_synapses_map.find(post_synaptic_neuron);
    if (map_iterator == neuron_synapses_map.end()) return;

    std::vector<Synapse*>& synapses = map_iterator -> second;

    auto vector_iterator = std::find(synapses.begin(), synapses.end(), synapse);

    if (vector_iterator != synapses.end()) {
      synapses.erase(vector_iterator);
    }
  }


  /**
   * @brief Upon call from a synapse which's weight has been updated normalize all synapses connected to post_synaptic_neuron
   * Synapse MUST have w_max param, get_w_max, get_weight and set_weight methods.
   */
  void normalize_weights(Synapse* updated_synapse, Neuron* post_synaptic_neuron) requires NormalizableSynapseConcept<Synapse>{
    auto map_iterator = neuron_synapses_map.find(post_synaptic_neuron);
    if (map_iterator == neuron_synapses_map.end()) return;

    std::vector<Synapse*>& synapse_group = map_iterator->second;
    if (synapse_group.empty()) return;

    const precission w_max = updated_synapse->get_w_max();
    const precission w_min = -w_max;
    precission range = w_max - w_min;

    // Subtract the mean to keep sum constant
    precission sum = 0;
    for (Synapse* s : synapse_group) {
        sum += s->get_weight();
    }
    
    precission mean = sum / synapse_group.size();

    if (std::abs(mean) > 1e-15) {
        for (Synapse* s : synapse_group) {
            precission w = s->get_weight();
            s->set_weight(w - mean);
        }
    }

    // Values must stay in range [-w_max, w_max]
    for (Synapse* s : synapse_group) {
        precission w = s->get_weight();
        if (w > w_max) s->set_weight(w_max);
        else if (w < -w_max) s->set_weight(-w_max);
    }
  }
 private:

 // Private constructor (Singleton)
  SynapseWeightNormalizer() = default;

  // Map to store the synapses connected to a neuron
  std::map<Neuron*, std::vector<Synapse*>> neuron_synapses_map;

};

#endif /*SYNAPSEWEIGHTNORMALIZER_H_*/
