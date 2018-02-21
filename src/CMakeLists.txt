cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)


add_executable(reciver
        main.cpp
        TCPServer.cpp
        TCPServer.h Session.cpp Session.h)

find_package(Boost 1.40 COMPONENTS system REQUIRED)
target_include_directories(reciver PUBLIC ${Bcppoost_INCLUDE_DIR})

target_link_libraries(reciver cxxopts ${Boost_LIBRARIES})