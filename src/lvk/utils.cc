#include "utils.h"
#include "exception.h"
#include "inlines.h"

#include <algorithm>
#include <set>
#include <vulkan/vk_enum_string_helper.h>

std::vector<VkLayerProperties> lvk::get_vulkan_layer_support() {
	uint32_t layer_cnt;
	std::vector<VkLayerProperties> layer_list;
	lvk_throw_if_failed(vkEnumerateInstanceLayerProperties(&layer_cnt, nullptr));
	if (layer_cnt) {
		layer_list.resize(layer_cnt);
		lvk_throw_if_failed(
			vkEnumerateInstanceLayerProperties(&layer_cnt, layer_list.data()));
	}
	return layer_list;
}

bool lvk::is_all_layers_supported(const std::vector<str> &layers) {
	std::set<str> set;
	for (const auto layer_list = get_vulkan_layer_support();
		 const auto &prop : layer_list)
		set.insert(prop.layerName);
	return std::ranges::all_of(layers, [&](str i) { return set.contains(i); });
}

std::string lvk::vk_result_to_string(VkResult result) {
	return string_VkResult(
		result); // todo: 非 class enum 和底层类型也被视为不同的类型，考虑重载
				 // std::to_string？
}

std::vector<VkPhysicalDevice>
lvk::get_vulkan_physical_devices(VkInstance const &instance_handle) {
	uint32_t count;
	std::vector<VkPhysicalDevice> devices;
	lvk_throw_if_failed(vkEnumeratePhysicalDevices(instance_handle, &count, nullptr));
	if (count) {
		devices.resize(count);
		lvk_throw_if_failed(
			vkEnumeratePhysicalDevices(instance_handle, &count, devices.data()));
	}
	return devices;
}

VkDevice lvk::create_vulkan_logical_device(
	const VkPhysicalDevice &physical_device,
	const VkPhysicalDeviceFeatures &enabled_features,
	const std::vector<VkDeviceQueueCreateInfo> &queue_create_infos,
	const std::vector<str> &enabled_extensions, const std::vector<str> &enabled_layers,
	const VkDeviceCreateFlags &flags) {
	const VkDeviceCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.flags = flags,
		.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
		.pQueueCreateInfos = queue_create_infos.data(),
		.enabledLayerCount = static_cast<uint32_t>(enabled_layers.size()),
		.ppEnabledLayerNames = enabled_layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(enabled_extensions.size()),
		.ppEnabledExtensionNames = enabled_extensions.data(),
		.pEnabledFeatures = &enabled_features,
	};
	VkDevice device;
	lvk_throw_if_failed_ex(vkCreateDevice(physical_device, &create_info,
										  default_vk_allocation_callbacks, &device),
						   "failed to create logical device");
	return device;
}
