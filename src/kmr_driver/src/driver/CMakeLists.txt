##############################################################################
# SOURCES
##############################################################################

file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

##############################################################################
# CONFIGURATIONS
##############################################################################

set(VERSION_FILE ${PROJECT_BINARY_DIR}/version_info.cpp)
configure_file(version_info.cpp.in ${VERSION_FILE} @ONLY)

##############################################################################
# LIBRARIES
##############################################################################

add_library(kmr ${SOURCES} ${VERSION_FILE})
target_link_libraries(kmr ${catkin_LIBRARIES})

install(TARGETS kmr
        DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

