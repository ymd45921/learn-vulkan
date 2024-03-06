#include "swapchain.h"
#include "device.h"
#include "../exception.h"
#include "../inlines.h"
#include "../vk-image.h"

#include <ranges>

namespace lvk {

	swapchain::swapchain(VkSwapchainKHR const &handle, device *base) :
		handle(handle), base(base) {}

	uint32_t swapchain::image_count() const {
		uint32_t count;
		lvk_throw_if_failed(vkGetSwapchainImagesKHR(**base, handle, &count, nullptr));
		return count;
	}

	swapchain swapchain::create(device *device,
	                            const VkSwapchainCreateInfoKHR &create_info) {
		VkSwapchainKHR handle;
		lvk_throw_if_failed_ex(
			vkCreateSwapchainKHR(**device, &create_info, default_vk_allocation_callbacks,
				&handle),
			"failed to create swapchain");
		return {handle, device};
	}

	swapchain::swapchain(swapchain &&old) noexcept {
		handle = old.handle, base = old.base;
		attachments = std::move(old.attachments);
		old.handle = VK_NULL_HANDLE;
	}

	swapchain::~swapchain() {
		if (base) {
			for (auto *buffer : attachments.framebuffers)
				vkDestroyFramebuffer(**base, buffer, default_vk_allocation_callbacks);
			for (auto *view : attachments.image_views)
				vkDestroyImageView(**base, view, default_vk_allocation_callbacks);
			if (handle != VK_NULL_HANDLE)
				vkDestroySwapchainKHR(**base, handle, default_vk_allocation_callbacks);
		}
	}

	std::vector<VkImage> swapchain::image_handles() const {
		auto count = image_count();
		std::vector<VkImage> images(count);
		lvk_throw_if_failed(
			vkGetSwapchainImagesKHR(**base, handle, &count, images.data()));
		return images;
	}

	void swapchain::create_image_views(VkImageViewCreateInfo create_info) {
		const auto images = image_handles();
		attachments.image_views.resize(images.size());
		for (size_t i = 0; i < images.size(); ++i) {
			create_info.image = images[i];
			lvk_throw_if_failed_ex(
				vkCreateImageView(**base, &create_info, default_vk_allocation_callbacks,
					&attachments.image_views[i]),
				"failed to create image view");
		}
	}

	void swapchain::create_framebuffers(
		const std::function<VkFramebufferCreateInfo(VkImageView image_view)>&
		create_info_generator) {
		attachments.framebuffers.resize(attachments.image_views.size());
		for (size_t i = 0; i < attachments.image_views.size(); ++i) {
			auto create_info = create_info_generator(attachments.image_views[i]);
			lvk_throw_if_failed_ex(
				vkCreateFramebuffer(**base, &create_info, default_vk_allocation_callbacks,
					&attachments.framebuffers[i]),
				"failed to create framebuffer");
		}
	}

	device &swapchain::parent_device() const { return *base; }


} // lvk
