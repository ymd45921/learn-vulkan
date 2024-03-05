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

		vk_image2d(const VkImage &handle, device *device,
				   VkDeviceMemory memory = VK_NULL_HANDLE);

		[[nodiscard]] VkDeviceMemory
		allocate_memory(const VkMemoryPropertyFlags &properties);

	public:
		static vk_image2d create(const VkImageCreateInfo &create_info, device *device,
								 const VkMemoryPropertyFlags
									 &properties); // todo: 每次都分配一块新的设备内存！

		VkImage operator*() const { return handle; }

		[[nodiscard]] VkMemoryRequirements memory_requirements() const;

		[[nodiscard]] VkDeviceMemory memory_handle() const;

		[[nodiscard]] device &parent_device() const;
	};

} // namespace lvk

#endif // LVK_VK_IMAGE_H
