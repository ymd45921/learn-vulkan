cmake_minimum_required(VERSION 3.25)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
include(cmake/setenv.cmake)

include_directories(src)

set(LVK_FILES src/lvk/exception.h src/lvk/exception.cc
	src/lvk/fwd.h src/lvk/inlines.h src/lvk/utils.h src/lvk/utils.cc
	src/lvk/wrappers/traits.h src/lvk/lvk.h src/lvk/lvk.cc src/lvk/helpers/helper-base.h
	src/lvk/window-app.cc src/lvk/window-app.h src/lvk/app.h
	src/lvk/objects/glfw-window.cc src/lvk/objects/glfw-window.h
	src/lvk/objects/device.cc src/lvk/objects/device.h
	src/lvk/vk-image.cc src/lvk/vk-image.h
	src/lvk/vk-buffer.cc src/lvk/vk-buffer.h
	src/lvk/objects/swapchain.cc src/lvk/objects/swapchain.h
	src/lvk/objects/instance.h src/lvk/objects/instance.cc
	src/lvk/objects/surface.h src/lvk/objects/surface.cc
	src/lvk/objects/physical-device.h src/lvk/objects/physical-device.cc
	src/lvk/objects/descriptor-set-layout.cc src/lvk/objects/descriptor-set-layout.h
	src/lvk/wrappers/surface-capabilities.cc src/lvk/wrappers/surface-capabilities.h
	src/lvk/vk-image-view.cc src/lvk/vk-image-view.h
	src/lvk/objects/render-pass.cc src/lvk/objects/render-pass.h
	src/lvk/shader.cc src/lvk/shader.h
	src/lvk/wrappers/application-info.cc src/lvk/wrappers/application-info.h
	src/lvk/helpers/pipeline-create-helper.cc src/lvk/helpers/pipeline-create-helper.h
)

set(MY_FILES src/my/utils.h src/my/utils.cc
	src/my/type-traits.h src/my/fwd.h
	src/my/fs.h src/my/fs.cc
	src/my/buffer.cc src/my/buffer.h
)

add_executable(${CMAKE_PROJECT_NAME}_MainApp
	${LVK_FILES} ${MY_FILES} src/main.cc)
target_link_vulkan(${CMAKE_PROJECT_NAME}_MainApp)
target_add_clang_format(${CMAKE_PROJECT_NAME}_MainApp)

add_subdirectory(test)