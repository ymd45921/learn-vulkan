#include "physical_device.h"
#include "lvk/lvk.h"

#include <set>

lvk::physical_device::physical_device(VkPhysicalDevice const &handle) : handle(handle) {}

lvk::physical_device lvk::physical_device::from_handle(VkPhysicalDevice const &h) {
	return lvk::physical_device(h);
}

VkPhysicalDeviceProperties lvk::physical_device::props() const {
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(handle, &props);
	return props;
}

VkPhysicalDeviceFeatures lvk::physical_device::features() const {
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(handle, &features);
	return features;
}

std::vector<VkQueueFamilyProperties> lvk::physical_device::queue_families_props() const {
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &count, nullptr);
	std::vector<VkQueueFamilyProperties> result(count);
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &count, result.data());
	return result;
}

std::vector<VkExtensionProperties> lvk::physical_device::extensions_props() const {
	uint32_t count;
	lvk_throw_if_failed_ex(vkEnumerateDeviceExtensionProperties(handle, nullptr, &count, nullptr),
						   "failed to get device extension properties count");
	std::vector<VkExtensionProperties> result(count);
	lvk_throw_if_failed_ex(
		vkEnumerateDeviceExtensionProperties(handle, nullptr, &count, result.data()),
		"failed to get device extension properties");
	return result;
}

bool lvk::physical_device::is_extension_supported(const std::vector<str> &extension_name) const {
	auto props = extensions_props();
	std::set<str> set;
	for (const auto &prop : props)
		set.insert(prop.extensionName);
	return std::ranges::all_of(extension_name, [&](str i) { return set.contains(i); });
}

VkSurfaceCapabilitiesKHR
lvk::physical_device::surface_capabilities(VkSurfaceKHR const &surface) const {
	VkSurfaceCapabilitiesKHR caps;
	lvk_throw_if_failed_ex(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, surface, &caps),
						   "failed to get surface capabilities");
	return caps;
}

std::vector<VkSurfaceFormatKHR>
lvk::physical_device::surface_formats(const VkSurfaceKHR &surface) const {
	uint32_t count;
	lvk_throw_if_failed_ex(vkGetPhysicalDeviceSurfaceFormatsKHR(handle, surface, &count, nullptr),
						   "failed to get surface formats count");
	std::vector<VkSurfaceFormatKHR> formats(count);
	lvk_throw_if_failed_ex(
		vkGetPhysicalDeviceSurfaceFormatsKHR(handle, surface, &count, formats.data()),
		"failed to get surface formats");
	return formats;
}

std::vector<VkPresentModeKHR>
lvk::physical_device::present_modes(const VkSurfaceKHR &surface) const {
	uint32_t count;
	lvk_throw_if_failed_ex(
		vkGetPhysicalDeviceSurfacePresentModesKHR(handle, surface, &count, nullptr),
		"failed to get present mode count");
	std::vector<VkPresentModeKHR> modes(count);
	lvk_throw_if_failed_ex(
		vkGetPhysicalDeviceSurfacePresentModesKHR(handle, surface, &count, modes.data()),
		"failed to get present mode");
	return modes;
}

