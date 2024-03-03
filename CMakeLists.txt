cmake_minimum_required(VERSION 3.25)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
include(cmake/setenv.cmake)

add_executable(${CMAKE_PROJECT_NAME}_MainApp
    src/main.cc)
target_link_vulkan(${CMAKE_PROJECT_NAME}_MainApp)