# Neun Library Tests

## Overview

This directory contains compile-time tests for the Neun library. These tests use C++20 concepts and static assertions to verify that all archetypes correctly implement their corresponding concepts.

## Test Philosophy

The Neun library uses **compile-time testing** through static assertions. This approach has several advantages:

1. **Zero runtime overhead**: Tests are validated at compile time, with no runtime execution
2. **Fast feedback**: Errors are caught immediately during compilation
3. **Type safety**: Ensures the entire type system is consistent
4. **Documentation**: Tests serve as executable documentation of concept requirements

## Test Files

### `test_concept_compliance.cpp` - Concept Compliance Tests

This is the main test file that validates all archetypes against their concepts. It is organized into sections:

#### Section 1: Basic Model and System Concepts
- `ModelArchetype` satisfies `ModelConcept`
- `SystemArchetype` satisfies `SystemConcept`
- Tests with both `double` and `float` precision

#### Section 2: Neuron Concepts
- `NeuronArchetype` satisfies `NeuronConcept`
- Inheritance validation (Neuron extends System)

#### Section 3: Integrator Concepts
- `IntegratorArchetype` satisfies `IntegratorConcept`
- Real integrators (`RungeKutta4`, `Euler`) also satisfy the concept
- Tests with different system types

#### Section 4: Dynamical System Concepts
- `DynamicalSystemArchetype` satisfies `DynamicalSystemConcept`
- `DifferentialDynamicalSystemArchetype` satisfies both dynamical and differential concepts

#### Section 5: Differential Neuron Concepts
- `DifferentialNeuronArchetype` satisfies neuron and differential concepts

#### Section 6: Integrated System Concepts
- `IntegratedSystemArchetype` satisfies `IntegratedSystemConcept`
- `IntegratableSystemConcept` validation

#### Section 7: Type Consistency
- Precision types are correctly defined
- Variable and parameter counts are accessible

## Building and Running Tests

### Build

```bash
cd build
cmake ..
make check_concepts
```

### What Happens

- CMake compiles the test files as OBJECT libraries (no linking)
- All `static_assert` statements are evaluated
- If compilation succeeds, **all tests pass**
- If compilation fails, the compiler will show which assertion failed

### Example Output (Success)

```
All concept compliance tests passed (compile-time validation)
```

## Adding New Tests

To add a new concept compliance test:

1. Include the concept and archetype headers
2. Add a `static_assert` with a descriptive message:

```cpp
static_assert(YourConcept<YourArchetype<double>>,
    "YourArchetype<double> must satisfy YourConcept");
```

3. Rebuild to verify

## Key Concepts in Neun

- **ModelConcept**: Basic model with variables, parameters, and eval function
- **SystemConcept**: Adds get/set methods for variables and parameters
- **NeuronConcept**: Extends System with synaptic input methods
- **IntegratorConcept**: Defines how to integrate a system over time
- **DynamicalSystemConcept**: System with step method for time evolution
- **DifferentialDynamicalSystemConcept**: Dynamical system using numerical integration

## Best Practices

1. **Always test with multiple precision types** (double, float)
2. **Test inheritance relationships** (e.g., Neuron extends System)
3. **Use descriptive assertion messages** for clear error reporting
4. **Organize tests by concept hierarchy** for maintainability
5. **Test real implementations** alongside archetypes when possible

## Troubleshooting

### Compilation Errors

If you see compilation errors in tests:

1. Read the assertion message carefully
2. Check which concept requirement is not satisfied
3. Verify the archetype or concept definition
4. Ensure all required members are public
5. Check type consistency (e.g., `precission_t` matches)

## Contributing

When adding new concepts or archetypes:

1. Define the concept in `concepts/`
2. Create an archetype in `archetypes/`
3. Add tests in `test_concept_compliance.cpp`
4. Ensure all assertions pass before committing

## References

- [C++20 Concepts](https://en.cppreference.com/w/cpp/language/constraints)
- [Neun Library Documentation](../README.md)
