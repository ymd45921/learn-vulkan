#include "swapchain.h"
#include "device.h"
#include "../exception.h"
#include "../inlines.h"
#include "../vk-image.h"

#include <ranges>

namespace lvk {

	swapchain::swapchain(VkSwapchainKHR const &handle, device *base)
		: handle(handle), base(base) { }

	swapchain swapchain::create(device *device, const VkSwapchainCreateInfoKHR &create_info) {
		VkSwapchainKHR handle;
		lvk_throw_if_failed_ex(vkCreateSwapchainKHR(**device, &create_info, default_vk_allocation_callbacks, &handle),
		                       "failed to create swapchain");
		return {handle, device};
	}

	swapchain::swapchain(swapchain &&old) noexcept {
		handle = old.handle, base = old.base;
		old.handle = VK_NULL_HANDLE;
	}

	swapchain::~swapchain() {
		if (handle != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(**base, handle, default_vk_allocation_callbacks);
		}
	}

	std::vector<VkImage> swapchain::image_handles() const {
		uint32_t count;
		lvk_throw_if_failed(vkGetSwapchainImagesKHR(**base, handle, &count, nullptr));
		std::vector<VkImage> images(count);
		lvk_throw_if_failed(vkGetSwapchainImagesKHR(**base, handle, &count, images.data()));
		return images;
	}

	device &swapchain::parent_device() const { return *base; }


} // lvk