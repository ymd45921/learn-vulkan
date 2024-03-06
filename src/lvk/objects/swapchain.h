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

		swapchain(const swapchain &) = delete;

		swapchain(swapchain &&) noexcept;

		~swapchain();

		[[nodiscard]] std::vector<VkImage> image_handles() const;

		[[nodiscard]] VkSwapchainKHR operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;

	};

} // lvk

#endif //LVK_SWAPCHAIN_H
