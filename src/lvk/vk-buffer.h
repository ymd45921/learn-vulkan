#ifndef LVK_VK_BUFFER_H
#define LVK_VK_BUFFER_H
#pragma once

#include "fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	class vk_buffer {

		VkBuffer handle;
		device *base;
		VkDeviceMemory memory;

		struct {
			VkDeviceSize size, offset;
			void *mapped;
		} record;

		vk_buffer(const VkBuffer &handle, device *device,
				  VkDeviceMemory memory = VK_NULL_HANDLE);

		// todo: 单个设备可以分配内存的次数有限！
		[[nodiscard]] VkDeviceMemory
		allocate_memory(const VkMemoryPropertyFlags &properties);

	public:
		static vk_buffer create(const VkBufferCreateInfo &create_info, device *device,
								const VkMemoryPropertyFlags &properties);

		vk_buffer(const vk_buffer &) = delete;

		vk_buffer(vk_buffer &&) noexcept;

		~vk_buffer() noexcept;

		VkDeviceMemory operator*() const { return memory; }

		[[nodiscard]] VkMemoryRequirements memory_requirements() const;

		[[nodiscard]] device &parent_device() const;

		void *map(VkMemoryMapFlags flags = 0);

		void unmap();
	};

} // namespace lvk

#endif // LVK_VK_BUFFER_H
