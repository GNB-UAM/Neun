set (INCLUDE_DIR ../include)
include_directories(${INCLUDE_DIR} ../concepts ../models ../integrators ../wrappers ../archetypes)

add_executable(basic basic.cpp)
target_link_libraries(basic)

add_executable(synapsis synapsis.cpp)
target_link_libraries(synapsis)

add_executable(chemicalSynapsis chemicalSynapsis.cpp)
target_link_libraries(chemicalSynapsis)

add_executable(linskerSynapse linskerSynapse.cpp)
target_link_libraries(linskerSynapse)

add_executable(STDPSynapse STDPSynapse.cpp)
target_link_libraries(STDPSynapse)
