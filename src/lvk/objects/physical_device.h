#ifndef LEARNVULKAN_PHYSICAL_DEVICE_H
#define LEARNVULKAN_PHYSICAL_DEVICE_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace lvk {

	/**
	 * @brief   Vulkan 物理设备 RAII 封装
	 * @details 用于查询物理设备的属性、特性、队列族等信息；不缓存查询结果
	 */
	class physical_device : public vk_object {

		VkPhysicalDevice handle;
		// ? 缓存设备属性和特性等需要查询的信息是否有必要？

	public:
		explicit physical_device(const VkPhysicalDevice &handle);

		VkPhysicalDevice operator*() const { return handle; }

		static physical_device from_handle(const VkPhysicalDevice &h);

		[[nodiscard]] VkPhysicalDeviceProperties props() const;

		[[nodiscard]] VkPhysicalDeviceFeatures features() const;

		[[nodiscard]] std::vector<VkQueueFamilyProperties> queue_families_props() const;

		[[nodiscard]] std::vector<VkExtensionProperties> extensions_props() const;

		[[nodiscard]] bool is_extension_supported(const std::vector<str> &extension_name) const;

		[[nodiscard]] VkSurfaceCapabilitiesKHR
		surface_capabilities(const VkSurfaceKHR &surface) const; // todo: 放在哪儿更合适？

		[[nodiscard]] std::vector<VkSurfaceFormatKHR>
		surface_formats(const VkSurfaceKHR &surface) const;

		[[nodiscard]] std::vector<VkPresentModeKHR>
		present_modes(const VkSurfaceKHR &surface) const;
	};
}

#endif // LEARNVULKAN_PHYSICAL_DEVICE_H
