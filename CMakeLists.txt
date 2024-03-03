cmake_minimum_required(VERSION 3.25)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
include(cmake/setenv.cmake)

include_directories(src)

set(LVK_FILES src/lvk/exception.h src/lvk/exception.cc
        src/lvk/fwd.h src/lvk/inlines.h src/lvk/utils.h src/lvk/utils.cc
        src/lvk/lvk.h src/lvk/lvk.cc
        src/lvk/window-app.cc
        src/lvk/window-app.h
        src/lvk/app.h
        src/lvk/objects/glfw-window.cc
        src/lvk/objects/glfw-window.h)

set(MY_FILES src/my/utils.h src/my/utils.cc
        src/lvk/objects/instance.h
        src/lvk/objects/instance.cc
        src/lvk/objects/surface.h
        src/lvk/objects/physical_device.h
        src/lvk/objects/physical_device.cc
        src/lvk/objects/surface.cc)

add_executable(${CMAKE_PROJECT_NAME}_MainApp
    ${LVK_FILES} ${MY_FILES} src/main.cc)
target_link_vulkan(${CMAKE_PROJECT_NAME}_MainApp)