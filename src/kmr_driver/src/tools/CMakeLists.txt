###############################################################################
# Tools
###############################################################################

add_executable(version_info version_info.cpp)
target_link_libraries(version_info kmr)

add_executable(simple_keyop simple_keyop.cpp)
target_link_libraries(simple_keyop kmr)

install(TARGETS version_info simple_keyop
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
