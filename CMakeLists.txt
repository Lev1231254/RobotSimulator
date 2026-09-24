cmake_minimum_required(VERSION 3.22)

project(RobotSimulator
    VERSION 0.1.0
    DESCRIPTION "A simple 2D robot simulator using SFML"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

add_executable(robot_simulator
    src/main.cpp
    src/draw.cpp
    src/robot.cpp
    src/utils.cpp
)

target_include_directories(robot_simulator
    PRIVATE
        include
)

target_link_libraries(robot_simulator
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

if (MSVC)
    target_compile_options(robot_simulator PRIVATE /W4)
else()
    target_compile_options(robot_simulator PRIVATE -Wall -Wextra -Wpedantic)
endif()