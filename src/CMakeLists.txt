cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)

add_executable(reciver main.cpp)

target_link_libraries(reciver cxxopts)