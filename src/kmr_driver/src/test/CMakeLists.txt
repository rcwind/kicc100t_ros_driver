###############################################################################
# Tests
###############################################################################

add_executable(kmr_velocity_commands velocity_commands.cpp)
target_link_libraries(kmr_velocity_commands kmr)

add_executable(demo_kmr_initialisation initialisation.cpp)
target_link_libraries(demo_kmr_initialisation kmr)

add_executable(demo_kmr_sigslots sigslots.cpp)
target_link_libraries(demo_kmr_sigslots kmr)

add_executable(demo_kmr_simple_loop simple_loop.cpp)
target_link_libraries(demo_kmr_simple_loop kmr)

install(TARGETS kmr_velocity_commands demo_kmr_initialisation demo_kmr_sigslots demo_kmr_simple_loop
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)