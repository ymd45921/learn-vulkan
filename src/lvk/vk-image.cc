#include "vk-image.h"
#include "exception.h"
#include "inlines.h"
#include "objects/device.h"
#include "objects/physical-device.h"
#include "vk-image-view.h"

#include <cmath>

namespace lvk {

	vk_image2d::vk_image2d(const VkImage &handle, device *device, VkDeviceMemory memory) :
		handle(handle), base(device), memory(memory), record() {}

	VkDeviceMemory vk_image2d::allocate_memory(const VkMemoryPropertyFlags &properties) {
		VkMemoryRequirements requirements = memory_requirements();
		VkMemoryAllocateInfo allocate_info = {.sType =
		                                      VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		                                      .allocationSize = requirements.size,
		                                      .memoryTypeIndex = 0};
		allocate_info.memoryTypeIndex = base->physical_device().pick_memory_type(
			requirements.memoryTypeBits, properties);
		vkAllocateMemory(**base, &allocate_info, default_vk_allocation_callbacks,
		                 &memory);
		return memory;
	}

	bool vk_image2d::is_format_linear_filter_support() const {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(*(base->physical_device()), record.format,
		                                    &properties);
		VkFormatFeatureFlags flags = 0;
		if (record.tiling == VK_IMAGE_TILING_LINEAR) {
			flags = properties.linearTilingFeatures;
		} else if (record.tiling == VK_IMAGE_TILING_OPTIMAL) {
			flags = properties.optimalTilingFeatures;
		}
		return (flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) != 0;
	}

	uint32_t vk_image2d::auto_mip_levels() const {
		uint32_t max_dim = std::max(record.extent.width, record.extent.height);
		return static_cast<uint32_t>(std::floor(std::log2(max_dim))) + 1;
	}

	vk_image2d
	vk_image2d::create(device *device, const VkImageCreateInfo &create_info,
	                   const VkMemoryPropertyFlags &properties) {
		VkImage handle;
		lvk_throw_if_failed_ex(vkCreateImage(**device, &create_info,
			                       default_vk_allocation_callbacks, &handle),
		                       "failed to create image");
		auto result = dry_create(handle, device, create_info);
		result.memory = result.allocate_memory(properties);
		lvk_throw_if_failed(vkBindImageMemory(**device, handle, result.memory, 0));
		return result;
	}

	vk_image2d vk_image2d::dry_create(const VkImage &handle, device *device,
	                                  const VkImageCreateInfo &create_info) {
		vk_image2d result(handle, device);
		result.record.layout = create_info.initialLayout;
		result.record.extent = create_info.extent;
		result.record.tiling = create_info.tiling;
		result.record.format = create_info.format;
		return result;
	}

	vk_image2d::vk_image2d(vk_image2d &&old) noexcept :
		handle(old.handle), base(old.base), memory(old.memory), record(old.record) {
		old.handle = VK_NULL_HANDLE, old.memory = VK_NULL_HANDLE;
	}

	vk_image2d::~vk_image2d() noexcept {
		if (handle != VK_NULL_HANDLE)
			vkDestroyImage(**base, handle, default_vk_allocation_callbacks);
		if (memory != VK_NULL_HANDLE)
			vkFreeMemory(**base, memory, default_vk_allocation_callbacks);
	}

	VkMemoryRequirements vk_image2d::memory_requirements() const {
		VkMemoryRequirements result;
		vkGetImageMemoryRequirements(**base, handle, &result);
		return result;
	}

	VkDeviceMemory vk_image2d::memory_handle() const { return memory; }

	device &vk_image2d::parent_device() const { return *base; }

	vk_image_view vk_image2d::
	create_view(const VkImageViewCreateInfo &create_info) const {
		auto create_info_copy = create_info;
		create_info_copy.image = handle;
		return vk_image_view::create(base, create_info_copy);
	}

} // namespace lvk
