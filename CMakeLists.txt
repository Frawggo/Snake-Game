cmake_minimum_required (VERSION 3.5)
project (Snake)

find_package(SFML COMPONENTS graphics window system REQUIRED)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp")
file (GLOB header_files "${source_dir}/*.h")

add_executable (Snake ${source_files} ${header_files})
target_link_libraries(Snake sfml-graphics sfml-window sfml-system)