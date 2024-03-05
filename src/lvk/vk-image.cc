#include "vk-image.h"
#include "exception.h"
#include "inlines.h"
#include "objects/device.h"
#include "objects/physical-device.h"


namespace lvk {

	vk_image2d::vk_image2d(const VkImage &handle, device *device, VkDeviceMemory memory) :
		handle(handle), base(device), memory(memory) {}

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

	vk_image2d vk_image2d::create(const VkImageCreateInfo &create_info, device *device,
								  const VkMemoryPropertyFlags &properties) {
		VkImage handle;
		lvk_throw_if_failed_ex(vkCreateImage(**device, &create_info,
											 default_vk_allocation_callbacks, &handle),
							   "failed to create image");
		vk_image2d result(handle, device);
		result.memory = result.allocate_memory(properties);
		vkBindImageMemory(**device, handle, result.memory, 0);
		return result;
	}

	VkMemoryRequirements vk_image2d::memory_requirements() const {
		VkMemoryRequirements result;
		vkGetImageMemoryRequirements(**base, handle, &result);
		return result;
	}

	VkDeviceMemory vk_image2d::memory_handle() const { return memory; }

	device &vk_image2d::parent_device() const { return *base; }

} // namespace lvk
