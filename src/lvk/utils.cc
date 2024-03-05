#include "utils.h"
#include "exception.h"
#include "inlines.h"

#include <algorithm>
#include <set>

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
	switch (result) {
	case 0: return "VK_SUCCESS";
	case 1: return "VK_NOT_READY";
	case 2: return "VK_TIMEOUT";
	case 3: return "VK_EVENT_SET";
	case 4: return "VK_EVENT_RESET";
	case 5: return "VK_INCOMPLETE";
	case -1: return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case -2: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case -3: return "VK_ERROR_INITIALIZATION_FAILED";
	case -4: return "VK_ERROR_DEVICE_LOST";
	case -5: return "VK_ERROR_MEMORY_MAP_FAILED";
	case -6: return "VK_ERROR_LAYER_NOT_PRESENT";
	case -7: return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case -8: return "VK_ERROR_FEATURE_NOT_PRESENT";
	case -9: return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case -10: return "VK_ERROR_TOO_MANY_OBJECTS";
	case -11: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case -12: return "VK_ERROR_FRAGMENTED_POOL";
	case -13: return "VK_ERROR_UNKNOWN";
	case -1000069000: return "VK_ERROR_OUT_OF_POOL_MEMORY";
	case -1000072003: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case -1000161000: return "VK_ERROR_FRAGMENTATION";
	case -1000257000: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case 1000297000: return "VK_PIPELINE_COMPILE_REQUIRED";
	case -1000000000: return "VK_ERROR_SURFACE_LOST_KHR";
	case -1000000001: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case 1000001003: return "VK_SUBOPTIMAL_KHR";
	case -1000001004: return "VK_ERROR_OUT_OF_DATE_KHR";
	case -1000003001: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case -1000011001: return "VK_ERROR_VALIDATION_FAILED_EXT";
	case -1000012000: return "VK_ERROR_INVALID_SHADER_NV";
	case -1000023000: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
	case -1000023001: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
	case -1000023002: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
	case -1000023003: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
	case -1000023004: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
	case -1000023005: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
	case -1000158000: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case -1000174001: return "VK_ERROR_NOT_PERMITTED_KHR";
	case -1000255000: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case 1000268000: return "VK_THREAD_IDLE_KHR";
	case 1000268001: return "VK_THREAD_DONE_KHR";
	case 1000268002: return "VK_OPERATION_DEFERRED_KHR";
	case 1000268003: return "VK_OPERATION_NOT_DEFERRED_KHR";
	case -1000299000: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
	case -1000338000: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
	case 1000482000: return "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";
	case 2147483647: return "VK_RESULT_MAX_ENUM";
	default: return "LVK_UNKNOWN_VK_RESULT";
	}
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
