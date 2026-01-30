# Neun

![CMake v3.5 dependency](https://img.shields.io/badge/cmake-v3.5-blue?style=flat-square)

A library for the simulation of dynamical systems, aimed to model neural
networks with high performance. It is written in an object oriented fashion
with heavily templated C++.

Forked from: https://code.launchpad.net/~elferdo/neun/trunk

## Build

To build it, just type:
```
mkdir build && cd build
cmake ..
make
```

Install it using:
```
make install
```

The library will install a pkg-config file called "neun.pc" under
${prefix}/${project_name}/${project_version}/pkgconfig. If you want
other applications to be able to find it, you must add this directory
to your PKG_CONFIG_PATH

<!--  
USING ECLIPSE

cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug .. 
-->


Note that Makefile has been generated in build directory and after running:
```
make 
```

two executable files will be generated in build/examples, corresponding to basic.cpp and synapse.cpp

## Usage

In order to perform any simulation first you need to define the numerical integrator you are going to use, e.g.:
```
typedef RungeKutta4 Integrator;
```

Then, you define the neuron model and the precision of the simulation, e.g.:
```
typedef HodgkinHuxleyModel<double> HHModel;
```

Next step, you wrap the model as a system to get memory for actual params and vars, and add the getters/setters, e.g.:
```
typedef SystemWrapper<HodgkinHuxleyModel<double>> HH
```

Finally, add the numerical integrator to build an integrable dynamical system, e.g.:
```
typedef DifferentialNeuronWrapper<HH, Integrator> Neuron;
```

Now you can use Neuron variables in your own code. In order to manually compile that code, from your project dir just run:
```
g++ -o mysimulation mysimulation.cpp -I/usr/local/Neun/<version>
````

### Integrators

Currently implemented integrators are:
 - Stepper
 - Euler
 - RungeKutta4
 - RungeKutta6

### Neuron models

Currently implemented neuron models:
 - Hodgkin-Huxley conductance model (Hodgkin and Huxley, 1952)
 - Hindmarsh–Rose model (Hindmarsh-Rose, 1984)
 - Izhikevich spiking neuron model (Izhikevich, 2003)
 - Simple oscillator
 - Matsuoka oscillator (Matsuoka, 1985)
 - Rowat and Selverston (Rowat and Selverston, 1997)
 - Rulkov Map model (Nikolai F. Rulkov, 2002)
 - Bistable Rulkov Map model (Nikolai F. Rulkov, 2002)
 - Vavoulis model (Vavoulis et al., 2007)

### Synapsis models

Currently implemented synapsis models are:
 - Diffusion synapsis (Destexhe et al. 1994)
 - Electrical synapsis
 - Conductance-based direct synapsis
 - Sigmoidal direct synapsis
