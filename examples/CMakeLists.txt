set (INCLUDE_DIR ../include)
include_directories(${INCLUDE_DIR} ../concepts ../models ../integrators ../wrappers ../archetypes)

add_executable(basic basic.cpp)
target_link_libraries(basic)

add_executable(synapse synapse.cpp)
target_link_libraries(synapse)
