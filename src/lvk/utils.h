#ifndef LVK_UTILS_H
#define LVK_UTILS_H
#pragma once

#include "fwd.h"

#include <vulkan/vulkan.h>
#include <string>

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
	bool is_all_layers_supported(const std::vector<str>& layers);

    /**
     * @brief   从 Vulkan 实例中获取 Vulkan 物理设备列表
     * @param instance_handle   Vulkan 实例句柄
     * @return  Vulkan 物理设备列表
     */
    std::vector<VkPhysicalDevice> get_vulkan_physical_devices(const VkInstance &instance_handle);

}

#endif //LVK_UTILS_H