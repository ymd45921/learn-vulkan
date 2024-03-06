#ifndef LVK_VK_IMAGE_VIEW_H
#define LVK_VK_IMAGE_VIEW_H
#pragma once

#include "fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	// ? 难道在我目前的工作逻辑中，这根本就不需要 RAII 封装？
	class vk_image_view {

		VkImageView handle;
		device *base;

		vk_image_view(const VkImageView &handle, device *base);

	public:
		static vk_image_view create(device *device, const VkImageViewCreateInfo &create_info);

		vk_image_view(const vk_image_view &) = delete;

		vk_image_view(vk_image_view &&) noexcept;

		~vk_image_view();

		[[nodiscard]] VkImageView operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;
	};

} // lvk

#endif //LVK_VK_IMAGE_VIEW_H
