#ifndef LVK_SWAPCHAIN_H
#define LVK_SWAPCHAIN_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>

namespace lvk {

	class swapchain : public vk_object {

		VkSwapchainKHR handle;
		device *base;

		struct { // ? mutable?
			std::vector<VkImageView> image_views;
			std::vector<VkFramebuffer> framebuffers;
		} attachments{};

		swapchain(const VkSwapchainKHR &handle, device *base);

		[[nodiscard]] uint32_t image_count() const;

		[[nodiscard]] std::vector<VkImage> image_handles() const;

		// * 现假设对每个图像都只创建一个视图；例外：VR 的场合，每个图像可能需要两个视图。
		void create_image_views(VkImageViewCreateInfo create_info);

		// ? 帧缓冲和视图确实是一一对应的吗？
		void create_framebuffers(
			const std::function<VkFramebufferCreateInfo(VkImageView image_view)>&
			create_info_generator);

	public:
		static swapchain create(device *device,
		                        const VkSwapchainCreateInfoKHR &create_info);

		swapchain(const swapchain &) = delete;

		swapchain(swapchain &&) noexcept;

		~swapchain();

		[[nodiscard]] VkSwapchainKHR operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;

	};

} // lvk

#endif //LVK_SWAPCHAIN_H
