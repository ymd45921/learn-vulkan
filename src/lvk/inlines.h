/**
 * 定义 lvk 命名空间内需要使用的默认值
 */
#ifndef LVK_INLINES_H
#define LVK_INLINES_H
#pragma once

#include "vulkan/vulkan.h"

#include <iostream>

namespace lvk {

	inline VkAllocationCallbacks *default_vk_allocation_callbacks = nullptr;

	inline VkBool32 default_debug_messenger_cb(
		[[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
		[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT msg_type,
		[[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT *p_cb_data,
		[[maybe_unused]] void *p_user_data) {
		std::cerr << "[DebugMessenger] " << p_cb_data->pMessage << std::endl;
		return VK_FALSE;
	}
} // namespace lvk

#endif // LVK_INLINES_H
