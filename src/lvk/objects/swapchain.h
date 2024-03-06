#ifndef LVK_SWAPCHAIN_H
#define LVK_SWAPCHAIN_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace lvk {

	class swapchain : public vk_object {

		VkSwapchainKHR handle;
		device *base;

		swapchain(const VkSwapchainKHR &handle, device *base);

	public:
		static swapchain create(device *device, const VkSwapchainCreateInfoKHR &create_info);

		[[nodiscard]] std::vector<VkImage> image_handles() const;

		[[nodiscard]] std::vector<vk_image2d>
		images(const VkSwapchainCreateInfoKHR &create_info) const; // BAD DESIGN!!!

		[[nodiscard]] VkSwapchainKHR operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;

	};

} // lvk

#endif //LVK_SWAPCHAIN_H
