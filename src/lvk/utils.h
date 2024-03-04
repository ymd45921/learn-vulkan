#ifndef LVK_UTILS_H
#define LVK_UTILS_H
#pragma once

#include "fwd.h"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace lvk {

	/**
	 * @brief   将 VkResult 转换为字符串
	 * @param result    VkResult 值
	 * @return  对应的字符串
	 */
	std::string vk_result_to_string(VkResult result);

	/**
	 * @brief   获取 Vulkan 层支持情况
	 * @return  一个包含 Vulkan 支持的所有层的列表
	 */
	std::vector<VkLayerProperties> get_vulkan_layer_support();

	/**
	 * @brief   检查指定的层是否被支持
	 * @param layers    待检查的层列表
	 * @return  如果所有层都被支持，则返回 true，否则返回 false
	 */
	bool is_all_layers_supported(const std::vector<str> &layers);

	/**
	 * @brief   从 Vulkan 实例中获取 Vulkan 物理设备列表
	 * @param instance_handle   Vulkan 实例句柄
	 * @return  Vulkan 物理设备列表
	 */
	std::vector<VkPhysicalDevice> get_vulkan_physical_devices(const VkInstance &instance_handle);

	/**
	 * @brief	创建 Vulkan 物理设备
	 * @param physical_device		要创建逻辑设备的 Vulkan 物理设备句柄
	 * @param enabled_features		要在 Vulkan 逻辑设备上启用的设备特性
	 * @param queue_create_infos	要在 Vulkan 逻辑设备上创建的队列族的创建信息
	 * @param enabled_extensions	要在 Vulkan 逻辑设备上启用的设备扩展
	 * @param enabled_layers		要在 Vulkan 逻辑设备上启用的层
	 * @param flags					创建逻辑设备的标志
	 * @return	新创建的 Vulkan 逻辑设备句柄
	 */
	[[nodiscard]] VkDevice create_vulkan_logical_device(
		const VkPhysicalDevice &physical_device,
		const VkPhysicalDeviceFeatures &enabled_features = {},
		const std::vector<VkDeviceQueueCreateInfo> &queue_create_infos = {},
		const std::vector<str> &enabled_extensions = {},
		const std::vector<str> &enabled_layers = {},
		const VkDeviceCreateFlags &flags = 0);

	/**
	 * @brief   获取 Vulkan 物理设备的队列族索引
	 * @param physical_device   Vulkan 物理设备句柄
	 * @param surface           Vulkan 表面句柄
	 * @return  包含队列族索引的结构体
	 */

}

#endif //LVK_UTILS_H
