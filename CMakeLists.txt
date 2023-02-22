cmake_minimum_required(VERSION 3.24)
project(cadmium_cbm)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(PARENT_DIR ../ ABSOLUTE)

add_executable(ogse_cadmium main.cpp )

target_include_directories(ogse_cadmium PUBLIC ${PARENT_DIR}/cadmium-2/include  ${PARENT_DIR}/cadmium-2/json/include)
target_include_directories(ogse_cadmium PUBLIC includes/)