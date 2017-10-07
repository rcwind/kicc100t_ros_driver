CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

# include_directories(${PROJECT_SOURCE_DIR}/inc)

GET_FILENAME_COMPONENT(this_dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB LIB_SRC "*.h" "*.c")
add_library("${this_dir_name}" ${LIB_SRC})

