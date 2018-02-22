cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)

add_executable(reciver
        main.cpp
        RawCmdDataProvider.cpp RawCmdDataProvider.h
        TCPServer.cpp TCPServer.h
        Session.cpp Session.h
        UserDataMgr.cpp UserDataMgr.h UserBid.cpp UserBid.h)

find_package(Boost COMPONENTS system REQUIRED)

target_include_directories(reciver PRIVATE
        ${Boost_INCLUDE_DIR}
        )

target_link_libraries(reciver
        pthread
        cxxopts
        ${Boost_SYSTEM_LIBRARY}
        )

