#ifndef LVK_DEVICE_H
#define LVK_DEVICE_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>

namespace lvk {

	/**
	 * @brief  Vulkan 逻辑设备 RAII 封装
	 */
	class device : public vk_object {

		VkDevice handle;
		physical_device *base;

		friend class physical_device;

		device(const VkDevice &handle, lvk::physical_device *base);

	public:

		// ? 如果它需要公开构造器，那么是在哪些场合需要？

		~device();

		VkDevice operator*() const { return handle; }

		[[nodiscard]] physical_device &physical_device() const;

		[[nodiscard]] VkQueue get_queue(uint32_t queue_family_index, uint32_t queue_index) const;

	};

} // lvk

#endif //LVK_DEVICE_H
