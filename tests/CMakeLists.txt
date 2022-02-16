cmake_minimum_required(VERSION 3.21)

enable_testing()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")

link_libraries(MolDyn)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

add_executable(test_matrix test_matrix.cpp)
add_executable(test_input_processing test_input_processing.cpp)
add_test(NAME test_matrix COMMAND test_matrix WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME test_input_processing COMMAND bash -c "test_input_processing input_processing_test.txt > output_processing_test.txt; test_input_processing < output_processing_test.txt | diff - output_processing_test.txt" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
