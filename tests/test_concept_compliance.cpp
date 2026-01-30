/*************************************************************
 * Neun Library - Concept Compliance Tests
 * 
 * This file contains compile-time static assertions to verify
 * that all archetypes satisfy their corresponding concepts.
 * 
 * These tests ensure:
 * 1. All archetypes are properly defined
 * 2. All concepts are correctly specified
 * 3. The type system is consistent across the library
 * 
 * If this file compiles successfully, all concept requirements
 * are satisfied.
 *************************************************************/

// Concept Headers
#include "ModelConcept.h"
#include "SystemConcept.h"
#include "NeuronConcept.h"
#include "DynamicalSystemConcept.h"
#include "DifferentialDynamicalSystemConcept.h"
#include "IntegratorConcept.h"
#include "IntegratableSystemConcept.h"
#include "IntegratedSystemConcept.h"

// Archetype Headers
#include "ModelArchetype.h"
#include "SystemArchetype.h"
#include "NeuronArchetype.h"
#include "DynamicalSystemArchetype.h"
#include "DifferentialDynamicalSystemArchetype.h"
#include "DifferentialNeuronArchetype.h"
#include "IntegratorArchetype.h"
#include "IntegratedSystemArchetype.h"

// Real Implementations (for additional validation)
#include "RungeKutta4.h"
#include "Euler.h"

//==============================================================================
// SECTION 1: Basic Model and System Concepts
//==============================================================================

// Test 1.1: ModelArchetype satisfies ModelConcept
static_assert(ModelConcept<ModelArchetype<double>>, 
    "ModelArchetype<double> must satisfy ModelConcept");

static_assert(ModelConcept<ModelArchetype<float>>, 
    "ModelArchetype<float> must satisfy ModelConcept");

// Test 1.2: SystemArchetype satisfies SystemConcept
static_assert(SystemConcept<SystemArchetype<double>>, 
    "SystemArchetype<double> must satisfy SystemConcept");

static_assert(SystemConcept<SystemArchetype<float>>, 
    "SystemArchetype<float> must satisfy SystemConcept");

//==============================================================================
// SECTION 2: Neuron Concepts
//==============================================================================

// Test 2.1: NeuronArchetype satisfies NeuronConcept (which extends SystemConcept)
static_assert(NeuronConcept<NeuronArchetype<double>>, 
    "NeuronArchetype<double> must satisfy NeuronConcept");

static_assert(NeuronConcept<NeuronArchetype<float>>, 
    "NeuronArchetype<float> must satisfy NeuronConcept");

// Test 2.2: NeuronArchetype also satisfies SystemConcept (inheritance)
static_assert(SystemConcept<NeuronArchetype<double>>, 
    "NeuronArchetype<double> must also satisfy SystemConcept");

//==============================================================================
// SECTION 3: Integrator Concepts
//==============================================================================

// Test 3.1: IntegratorArchetype satisfies IntegratorConcept
static_assert(IntegratorConcept<IntegratorArchetype, SystemArchetype<double>>,
    "IntegratorArchetype must satisfy IntegratorConcept with SystemArchetype<double>");

static_assert(IntegratorConcept<IntegratorArchetype, NeuronArchetype<double>>,
    "IntegratorArchetype must satisfy IntegratorConcept with NeuronArchetype<double>");

// Test 3.2: Real integrators satisfy IntegratorConcept
static_assert(IntegratorConcept<RungeKutta4, SystemArchetype<double>>,
    "RungeKutta4 must satisfy IntegratorConcept with SystemArchetype<double>");

static_assert(IntegratorConcept<Euler, SystemArchetype<double>>,
    "Euler must satisfy IntegratorConcept with SystemArchetype<double>");

//==============================================================================
// SECTION 4: Dynamical System Concepts
//==============================================================================

// Test 4.1: DynamicalSystemArchetype satisfies DynamicalSystemConcept
static_assert(DynamicalSystemConcept<DynamicalSystemArchetype<double>>, 
    "DynamicalSystemArchetype<double> must satisfy DynamicalSystemConcept");

// Test 4.2: DifferentialDynamicalSystemArchetype satisfies both concepts
static_assert(DynamicalSystemConcept<DifferentialDynamicalSystemArchetype<double>>, 
    "DifferentialDynamicalSystemArchetype<double> must satisfy DynamicalSystemConcept");

static_assert(DifferentialDynamicalSystemConcept<DifferentialDynamicalSystemArchetype<double>, IntegratorArchetype>, 
    "DifferentialDynamicalSystemArchetype<double> must satisfy DifferentialDynamicalSystemConcept");

//==============================================================================
// SECTION 5: Differential Neuron Concepts
//==============================================================================

// Test 5.1: DifferentialNeuronArchetype satisfies required concepts
static_assert(NeuronConcept<DifferentialNeuronArchetype<double>>, 
    "DifferentialNeuronArchetype<double> must satisfy NeuronConcept");

static_assert(DifferentialDynamicalSystemConcept<DifferentialNeuronArchetype<double>, IntegratorArchetype>, 
    "DifferentialNeuronArchetype<double> must satisfy DifferentialDynamicalSystemConcept");

//==============================================================================
// SECTION 6: Integrated System Concepts
//==============================================================================

// Test 6.1: IntegratedSystemArchetype satisfies IntegratedSystemConcept
static_assert(IntegratedSystemConcept<IntegratedSystemArchetype<double>>, 
    "IntegratedSystemArchetype<double> must satisfy IntegratedSystemConcept");

// Test 6.2: Verify IntegratableSystemConcept is satisfied by appropriate types
static_assert(IntegratableSystemConcept<SystemArchetype<double>>, 
    "SystemArchetype<double> must satisfy IntegratableSystemConcept");

//==============================================================================
// SECTION 7: Type Consistency Checks
//==============================================================================

// Test 7.1: Precision types are correctly defined
static_assert(std::is_same_v<ModelArchetype<double>::precission_t, double>,
    "ModelArchetype<double>::precission_t must be double");

static_assert(std::is_same_v<SystemArchetype<float>::precission_t, float>,
    "SystemArchetype<float>::precission_t must be float");

// Test 7.2: Variable and parameter counts are accessible
static_assert(ModelArchetype<double>::n_variables > 0,
    "ModelArchetype must have at least one variable");

static_assert(ModelArchetype<double>::n_parameters > 0,
    "ModelArchetype must have at least one parameter");

//==============================================================================
// Test Completion
//==============================================================================

// If compilation reaches here, all concept checks have passed
int main() {
    return 0;  // Success - all static assertions passed
}
