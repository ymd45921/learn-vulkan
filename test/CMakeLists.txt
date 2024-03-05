add_library(${CMAKE_PROJECT_NAME}_TestLib STATIC
	type.cc)
target_link_vulkan(${CMAKE_PROJECT_NAME}_TestLib)