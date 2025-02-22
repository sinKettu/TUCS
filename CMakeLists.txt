cmake_minimum_required(VERSION 3.10)

if(TYPE STREQUAL "server")
    message("You chose to build the server")
    project(test_server)
    set(SOURCE_FILES Server/main.cpp
            Server/server.h Server/server.cpp
            Server/tcp_server.h Server/tcp_server.cpp
            Server/udp_server.h Server/udp_server.cpp)
    add_executable(test_server ${SOURCE_FILES})
elseif(TYPE STREQUAL "client")
    message("You chose to build the client")
    project(test_client)
    set(SOURCE_FILES Client/main.cpp
            Client/client.h Client/client.cpp)
    add_executable(test_client ${SOURCE_FILES})
else()
    message( FATAL_ERROR "You have to select TYPE: server or client!\ncmake -DTYPE=server OR cmake -DTYPE=client")
endif()

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
