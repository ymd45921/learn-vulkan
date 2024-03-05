#ifndef LVK_SURFACE_H
#define LVK_SURFACE_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	/**
	 * @brief   Vulkan 表面对象
	 */
	class surface : public vk_object {

		VkInstance dep;
		VkSurfaceKHR handle;

		surface(VkSurfaceKHR handle, VkInstance dep);

	public:
		[[deprecated]] surface();

		static surface create(const instance &instance, const glfw_window &window);

		~surface(); // ? 如何表现 Surface 对于 Instance 的依赖关系？

		VkSurfaceKHR operator*() const { return handle; }
	};

} // namespace lvk

#endif // LVK_SURFACE_H
