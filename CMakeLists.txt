cmake_minimum_required(VERSION 3.15)
project(ecu_control)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")

find_package(PythonLibs REQUIRED)

include_directories(
    ${PYTHON_INCLUDE_DIRS}
)

add_executable(ECU_Control
        src/main.cpp
        src/ReadCANThread.c
        src/SteeringBuffer.c
        src/ThrottleBuffer.c
        src/ControlThread.cpp
)
target_link_libraries(ECU_Control ${PYTHON_LIBRARIES})
