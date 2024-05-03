cmake_minimum_required(VERSION 3.10)
project(CAN_Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dbcppp/src)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dbcppp/build)

find_package(Boost REQUIRED)

add_executable(can_receiver can_receiver.cpp)
target_link_libraries(can_receiver dbcppp ${Boost_LIBRARIES})

add_executable(can_sender can_sender.cpp)
target_link_libraries(can_sender dbcppp ${Boost_LIBRARIES})
