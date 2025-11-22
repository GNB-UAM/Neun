# Neun

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square)
![CMake v3.5](https://img.shields.io/badge/cmake-v3.5-blue?style=flat-square)
![Header Only](https://img.shields.io/badge/header--only-yes-green?style=flat-square)
![License](https://img.shields.io/badge/license-BSD-blue?style=flat-square)

A modern, header-only C++20 library for the simulation of dynamical systems, designed for modeling neural networks with high performance. Built on C++20 concepts for type safety and compile-time validation.

Forked from: https://code.launchpad.net/~elferdo/neun/trunk

## Features

- **Header-only**: No compilation required, just include and use
- **C++20 concepts**: Type-safe interfaces with clear concept requirements
- **Zero runtime overhead**: All concept checks happen at compile time
- **Template-based**: Highly optimized generic code
- **Modular design**: Mix and match models, integrators, and wrappers
- **Extensive model library**: Wide range of neuron and synapse models

## Requirements

- **C++20 compatible compiler**: GCC 10+, Clang 10+, or MSVC 2019+
- **CMake 3.5+** (for building tests and installation)

## Installation

### Option 1: Direct Copy (Simplest for header-only)

Since Neun is header-only, you can simply copy the headers:

```bash
git clone <repository-url>
cd neun
# Copy to your project
cp -r include/ integrators/ concepts/ archetypes/ models/ wrappers/ /your/project/include/neun/

# Or install system-wide
sudo cp -r include/ integrators/ concepts/ archetypes/ models/ wrappers/ /usr/local/include/neun/
```

### Option 2: CMake Installation (Recommended for integration)

If you want CMake package support (allows `find_package(neun)`):

```bash
git clone <repository-url>
cd neun
mkdir build && cd build
cmake ..
sudo make install
```

This installs headers to `/usr/local/include/neun/` and creates CMake config files for easy integration.

**Note**: Since Neun is header-only, the "build" step only validates concept compliance tests. No compilation of the library itself occurs.

### Option 3: Using CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    neun
    GIT_REPOSITORY <repository-url>
    GIT_TAG main
)
FetchContent_MakeAvailable(neun)

target_link_libraries(your_target PRIVATE neun::neun)
```

### Option 4: Using CMake find_package (after CMake install)

```cmake
find_package(neun REQUIRED)
target_link_libraries(your_target PRIVATE neun::neun)
```

## Quick Start

### Basic Usage

```cpp
#include <iostream>
#include "IzhikevichModel.h"
#include "SystemWrapper.h"
#include "IzhikevichSystemWrapper.h"
#include "IntegratedSystemWrapper.h"
#include "RungeKutta4.h"

int main() {
    // Define your integrator
    typedef RungeKutta4 Integrator;
    
    // Create a neuron with proper wrapping
    typedef IntegratedSystemWrapper<
        IzhikevichSystemWrapper<double>, 
        Integrator
    > Neuron;
    
    // Configure the neuron
    Neuron::ConstructorArgs args;
    args.params[Neuron::parameter::a] = 0.02;
    args.params[Neuron::parameter::b] = 0.2;
    args.params[Neuron::parameter::c] = -65.0;
    args.params[Neuron::parameter::d] = 8.0;
    args.params[Neuron::parameter::threshold] = 30.0;
    
    Neuron neuron(args);
    
    // Set initial conditions
    neuron.set(Neuron::variable::v, -65.0);
    neuron.set(Neuron::variable::u, neuron.get(Neuron::parameter::b) 
                                     * neuron.get(Neuron::variable::v));
    
    // Simulate
    double dt = 0.1;  // ms
    double t_max = 100.0;  // ms
    
    for (double t = 0; t < t_max; t += dt) {
        neuron.step(dt);
        std::cout << t << " " << neuron.get(Neuron::variable::v) << std::endl;
    }
    
    return 0;
}
```

### Compile and Run

```bash
g++ -std=c++20 -O3 -I/usr/local/include/neun simulation.cpp -o simulation
./simulation
```

## Architecture

### Concepts

Neun uses C++20 concepts to define clear interfaces:

- **ModelConcept**: Basic model with eval() method
- **SystemConcept**: Adds get/set for variables and parameters  
- **NeuronConcept**: Extends System with synaptic inputs
- **IntegratorConcept**: Numerical integration interface
- **DynamicalSystemConcept**: Time evolution with step()
- **IntegratableSystemConcept**: System ready for integration

### Wrappers

Wrappers provide functionality layers:

- **SystemWrapper**: Adds get/set interface to models
- **DynamicalSystemWrapper**: Adds time evolution
- **DifferentialNeuronWrapper**: Combines neuron + integrator
- **IntegratedSystemWrapper**: Full integration pipeline
- **IzhikevichSystemWrapper**: Model-specific reset logic

### Available Integrators

- **Stepper**: Fixed step integrator base
- **Euler**: First-order Euler method
- **RungeKutta4**: Fourth-order Runge-Kutta (RK4)
- **RungeKutta6**: Sixth-order Runge-Kutta (RK6)

### Neuron Models

#### Spiking Neuron Models
- **HodgkinHuxleyModel**: Conductance-based model (Hodgkin & Huxley, 1952)
- **IzhikevichModel**: Simplified spiking model (Izhikevich, 2003)
- **HindmarshRoseModel**: Bursting neuron (Hindmarsh & Rose, 1984)
- **VavoulisModel**: Detailed conductance model (Vavoulis et al., 2007)
- **VavoulisCGCModel**: Vavoulis with calcium-gated currents
- **VavoulisCGCModelQ10**: Temperature-dependent variant

| Model | Class Name | Description |
|-------|------------|-------------|
| **Hodgkin-Huxley** | HodgkinHuxleyModel | Classic conductance-based model |
| **Hindmarsh-Rose** | HindmarshRoseModel | Simplified bursting model |
| **Izhikevich**     | IzhikevichModel    | Spiking neuron model (Izhikevich, 2003) |
| **Vavoulis**       |  VavoulisModel  | Detailed conductance model (Vavoulis et al., 2007) |
| **Vavoulis CGC**|  VavoulisCGCModel  | Vavoulis with calcium-gated currents |
| **Vavoulis CGC Q10**|  VavoulisCGCModelQ10  | Temperature-dependent variant |

#### Map-Based Models
| Model | Class Name | Description |
|-------|------------|-------------|
| **Rulkov**                 |   RulkovMapModel   | Discrete-time neuron (Rulkov, 2002) |
| **BistableRulkovMapModel** |   BistableRulkovMapModel  | Bistable variant |

#### Oscillator Models
| Model | Class Name | Description |
|-------|------------|-------------|
| **Simple oscillator| SimpleOscillatorModel    | Basic oscillator |
| **Matsuoka**       | MatsuokaOscillatorModel  | CPG oscillator (Matsuoka, 1985) |
| **RowatSelverston**| RowatSelverstonModel     | Rowat and Selverston model (1997) |

### Synapsis
| Model | Class Name | Description |
|-------|------------|-------------|
| **DiffusionSynapsis** | DiffusionSynapsisModel    | Chemical synapse (Destexhe et al., 1994) |
| **Gradual activation synapse**       | GradualActivationSynapsisModel  | Gradual activation model |
| **Gap junction**| ElectricalSynapsis     | Gap junction |
| **Direct synapsis**| DirectSynapsis     | Direct conductance coupling |
| **Sigmoidal activation**| SigmoidalDirectSynapsis     | Sigmoidal activation |

### Utility Components

- **CurrentPulse**: Inject current pulses
- **CurrentSource**: Custom current injection
- **NeuronBase**: Base class for neuron models
- **ModelBase**: Base class for all models

## Testing

The library includes comprehensive compile-time tests:

```bash
cd build
cmake ..
make check_concepts
```

Tests verify that all archetypes satisfy their concepts. See [tests/README.md](tests/README.md) for details.

## Design Philosophy

### Type Safety

All interfaces are enforced through C++20 concepts, providing:
- Clear compiler error messages
- Self-documenting code
- Compile-time validation

### Zero Overhead

All abstractions compile away:
- No virtual functions (except where explicitly needed)
- Template-based polymorphism
- Inline everything

### Composability

Mix and match components:
- Any model with any integrator
- Multiple wrapper layers
- Custom wrappers for specific models

## Examples

Examples have been moved to a separate repository to keep this library focused on the core functionality. See the [examples repository](github.com/GNB-UAM/neun-examples) for complete working examples.

## Documentation

- **Library overview**: This README
- **Testing guide**: [tests/README.md](tests/README.md)
<!-- - **API documentation**: coming soon -->
- **Concept reference**: See headers in `concepts/`

## Contributing

Contributions are welcome! When adding new features:

1. Define appropriate concepts in `concepts/`
2. Create archetypes in `archetypes/`
3. Add tests in `tests/test_concept_compliance.cpp`
4. Ensure all concept assertions pass
5. Document public interfaces

## Performance

Neun is designed for high performance:
- Zero-overhead abstractions
- Header-only means no library linking overhead
- Aggressive inlining
- Template specialization opportunities

Typical performance: 10,000+ neurons at >1kHz real-time on modern hardware.

## License

BSD 3-Clause License. See [Copyright](Copyright) file for details.

## Citation

If you use Neun in your research, please cite:

```
@software{neun2024,
  title={Neun: A C++20 Library for Neural Network Simulation},
  author={Lareo, Angel and Herrero Carrón, Fernando},
  year={2024},
  url={https://github.com/GNB-UAM/neun}
}
```

## Contact

- **Maintainer**: Angel Lareo (angel.lareo@uam.es)
- **Organization**: Grupo de Neurocomputación Biológica, UAM
- **Issues**: Use GitHub issue tracker

## Acknowledgments

Original implementation by Fernando Herrero Carrón. Modernized and refactored by Angel Lareo.