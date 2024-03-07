#include "surface.h"
#include "glfw-window.h"
#include "instance.h"

#include "../exception.h"

lvk::surface::surface(instance *dep, VkSurfaceKHR handle) : handle(handle), base(dep) {}

lvk::surface::surface(surface &&old) noexcept {
	base = old.base, handle = old.handle;
	old.handle = VK_NULL_HANDLE;
}

lvk::surface lvk::surface::create(lvk::instance *instance,
                                  const lvk::glfw_window &window) {
	VkSurfaceKHR surface;
	if (**instance == VK_NULL_HANDLE) {
		throw lvk::vk_exception(
			"lvk::surface::create: Vulkan instance passed is not initialized");
	}
	lvk_throw_if_failed_ex(glfwCreateWindowSurface(**instance, *window, nullptr, &surface),
						   "Failed to create window surface");
	return {instance, surface};
}

lvk::surface::surface() : base(nullptr), handle(VK_NULL_HANDLE) {}

lvk::surface::~surface() {
	if (handle != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(**base, handle, nullptr);
	}
}
