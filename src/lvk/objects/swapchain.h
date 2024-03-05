#ifndef LVK_SWAPCHAIN_H
#define LVK_SWAPCHAIN_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	class swapchain : public vk_object {

		VkSwapchainKHR handle;
		device *base;

		swapchain(const VkSwapchainKHR &handle, device *base);
	};

} // lvk

#endif //LVK_SWAPCHAIN_H
