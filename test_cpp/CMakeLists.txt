cmake_minimum_required(VERSION 3.10)
project(CAN_Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dbcppp/src)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dbcppp/build)

add_executable(can_receiver can_receiver.cpp)

find_package(Boost REQUIRED)
target_link_libraries(can_receiver dbcppp ${Boost_LIBRARIES})
