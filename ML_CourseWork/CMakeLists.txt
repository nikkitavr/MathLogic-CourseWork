cmake_minimum_required(VERSION 3.20)
project(ML_CourseWork)

set(CMAKE_CXX_STANDARD 14)

#add_executable(ML_CourseWork main.cpp bdd.h)
add_subdirectory (external/buddy bdd)

add_executable(ML_CourseWork main.cpp Utils.cpp)
#add_executable(ML_CourseWork main_test.cpp Utils.cpp)
target_include_directories(ML_CourseWork PRIVATE "${PROJECT_SOURCE_DIR}/external/buddy/src")
target_link_libraries(ML_CourseWork bdd)