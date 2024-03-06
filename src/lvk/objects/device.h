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
		static device create(physical_device *physical_device,
							 const VkDeviceCreateInfo &create_info);

		device(const device &) = delete;

		device(device &&) noexcept;

		~device();

		VkDevice operator*() const { return handle; }

		[[nodiscard]] physical_device &physical_device() const;

		[[nodiscard]] VkQueue get_queue(uint32_t queue_family_index,
										uint32_t queue_index) const;
	};

} // namespace lvk

#endif // LVK_DEVICE_H
