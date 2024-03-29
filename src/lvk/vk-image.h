#ifndef LVK_VK_IMAGE_H
#define LVK_VK_IMAGE_H
#pragma once

#include "fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	class vk_image2d {

		VkImage handle;
		device *base;
		VkDeviceMemory memory;

		struct {
			VkImageLayout layout;
			VkExtent3D extent;
			VkImageTiling tiling;
			VkFormat format;
		} record;

		vk_image2d(const VkImage &handle, device *device,
		           VkDeviceMemory memory = VK_NULL_HANDLE);

		[[nodiscard]] VkDeviceMemory
		allocate_memory(const VkMemoryPropertyFlags &properties);

		// ? 疑似有点过于特殊化了
		[[nodiscard]] bool is_format_linear_filter_support() const;

		[[nodiscard]] uint32_t auto_mip_levels() const;

		inline static vk_image2d // BAD DESIGN!!!
		dry_create(const VkImage &handle, device *device, const VkImageCreateInfo &create_info);

	public:
		static vk_image2d
		create(device *device, const VkImageCreateInfo &create_info,
		       const VkMemoryPropertyFlags &properties); // todo: 每次都分配一块新的设备内存！

		vk_image2d(const vk_image2d &) = delete;

		vk_image2d(vk_image2d &&) noexcept;

		~vk_image2d() noexcept;

		VkImage operator*() const { return handle; }

		[[nodiscard]] VkMemoryRequirements memory_requirements() const;

		[[nodiscard]] VkDeviceMemory memory_handle() const;

		[[nodiscard]] device &parent_device() const;

		[[nodiscard]] vk_image_view create_view(const VkImageViewCreateInfo &create_info) const;

	};

} // namespace lvk

#endif // LVK_VK_IMAGE_H
