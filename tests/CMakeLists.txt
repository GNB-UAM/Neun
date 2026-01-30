# Neun Library - Concept Compliance Tests
#
# These tests are compile-time only - they verify that all archetypes
# satisfy their corresponding concepts through static_assert.
# 
# We use OBJECT libraries to compile but not link, ensuring:
# 1. All static_asserts are checked
# 2. No executable is produced (tests are purely compile-time)
# 3. Fast incremental builds

# Include directories for header-only library
set(INCLUDE_DIR ../include)
include_directories(
    ${INCLUDE_DIR} 
    ../concepts 
    ../models 
    ../integrators 
    ../wrappers 
    ../archetypes
)

# Test 01: Comprehensive concept compliance testing
# Validates all archetypes against their concepts
add_library(test_concept_compliance OBJECT test_concept_compliance.cpp)
target_compile_features(test_concept_compliance PRIVATE cxx_std_20)

# Custom target to run all concept tests
add_custom_target(check_concepts
    DEPENDS test_concept_compliance
    COMMENT "All concept compliance tests passed (compile-time validation)"
)
