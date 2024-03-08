#include "lvk/objects/instance.h"
#include "lvk/inlines.h"
#include "lvk/utils.h"
#include "lvk/lvk.h"
#include "lvk/objects/physical-device.h"
#include "lvk/wrappers/application-info.h"

#include <ranges>

lvk::instance::instance(std::string_view app_name, std::vector<str> enabled_extensions,
                        std::vector<str> enabled_layers, unsigned flags) {
	VkApplicationInfo app_info = wrappers::application_info::create(app_name.data());
#ifdef VULKAN_EXPLICIT_USE_MOLTENVK
	enabled_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	enabled_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
#ifdef LearnVulkan_debug
	enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
	enabled_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	if (!is_all_layers_supported(enabled_layers)) {
		throw vk_exception("lvk::instance: not all required layers are supported.");
	}
	const VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.flags = flags,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = static_cast<uint32_t>(enabled_layers.size()),
		.ppEnabledLayerNames = enabled_layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(enabled_extensions.size()),
		.ppEnabledExtensionNames = enabled_extensions.data(),
	};
	VkInstance instance;
	lvk_throw_if_failed_ex(
		vkCreateInstance(&create_info, default_vk_allocation_callbacks, &instance),
		"failed to create instance");
	handle = instance;
	debug_messenger = nullptr;
#ifdef LearnVulkan_debug
	constexpr VkDebugUtilsMessengerCreateInfoEXT setup_debug_messenger_create_info = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = default_debug_messenger_cb,
		.pUserData = nullptr};
	lvk_throw_if_failed(lvk_load_vulkan_api(handle, vkCreateDebugUtilsMessengerEXT)(
		handle, &setup_debug_messenger_create_info, default_vk_allocation_callbacks,
		&debug_messenger));
#endif
}

lvk::instance::instance(instance &&old) noexcept {
	handle = old.handle;
#ifdef LearnVulkan_debug
	debug_messenger = old.debug_messenger;
	old.debug_messenger = VK_NULL_HANDLE;
#endif
	old.handle = VK_NULL_HANDLE;
}

lvk::instance::~instance() {
#ifdef LearnVulkan_debug
	if (debug_messenger != VK_NULL_HANDLE)
		lvk_load_vulkan_api(handle, vkDestroyDebugUtilsMessengerEXT)(
			handle, debug_messenger, default_vk_allocation_callbacks);
#endif
	if (handle != VK_NULL_HANDLE)
		vkDestroyInstance(handle, default_vk_allocation_callbacks);
}

std::vector<lvk::physical_device> lvk::instance::get_physical_devices() const {
	auto devices = get_vulkan_physical_devices(**this);
	auto result = devices | std::ranges::views::transform(physical_device::from_handle);
	return {result.begin(), result.end()};
}

lvk::physical_device
lvk::instance::pick_physical_device(const std::vector<str> &required_extensions) const {
	auto devices = get_physical_devices();
	auto result = std::ranges::find_if(devices, [&](const auto &device) {
		return device.is_extension_supported(required_extensions);
	});
	if (result == devices.end()) {
		throw vk_exception("lvk::instance::pick_physical_device: no suitable "
			"physical device found.");
	}
	return *result;
}
