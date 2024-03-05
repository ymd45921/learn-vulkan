#include "surface.h"
#include "glfw-window.h"
#include "instance.h"

#include "../exception.h"

lvk::surface::surface(VkSurfaceKHR handle, VkInstance dep) : handle(handle), dep(dep) {}

lvk::surface lvk::surface::create(const lvk::instance &instance,
								  const lvk::glfw_window &window) {
	VkSurfaceKHR surface;
	if (*instance == VK_NULL_HANDLE) {
		throw lvk::vk_exception(
			"lvk::surface::create: Vulkan instance passed is not initialized");
	}
	lvk_throw_if_failed_ex(glfwCreateWindowSurface(*instance, *window, nullptr, &surface),
						   "Failed to create window surface");
	return {surface, *instance};
}

lvk::surface::surface() : handle(VK_NULL_HANDLE), dep(VK_NULL_HANDLE) {}

lvk::surface::~surface() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(dep, handle, nullptr);
	}
}
