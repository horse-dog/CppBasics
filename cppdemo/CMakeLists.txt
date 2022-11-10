cmake_minimum_required(VERSION 3.15)
project(cppDemo)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(/opt/homebrew/Cellar/boost/1.79.0_1/include)

aux_source_directory(. SRC_LIST)
foreach(src ${SRC_LIST})
    string(REPLACE "./" "" target ${src})
    string(REPLACE ".cc" "" target ${target})
    add_executable(${target} ${src})
endforeach()

add_executable(tcp_client socket/tcp_client.cc)
add_executable(tcp_server socket/tcp_server.cc)
add_executable(udp_client socket/udp_client.cc)
add_executable(udp_server socket/udp_server.cc)