#include "vk-buffer.h"
#include "exception.h"
#include "inlines.h"
#include "objects/device.h"
#include "objects/physical-device.h"

namespace lvk {

	vk_buffer::vk_buffer(const VkBuffer &handle, device *device, VkDeviceMemory memory) :
		handle(handle), base(device), memory(memory), record() {}

	VkDeviceMemory vk_buffer::allocate_memory(const VkMemoryPropertyFlags &properties) {
		VkMemoryRequirements requirements = memory_requirements();
		VkMemoryAllocateInfo allocate_info = {.sType =
		                                      VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		                                      .allocationSize = requirements.size,
		                                      .memoryTypeIndex = 0};
		allocate_info.memoryTypeIndex = base->physical_device().pick_memory_type(
			requirements.memoryTypeBits, properties);
		lvk_throw_if_failed_ex(vkAllocateMemory(**base, &allocate_info,
			                       default_vk_allocation_callbacks, &memory),
		                       "failed to allocate memory for buffer");
		return memory;
	}

	vk_buffer vk_buffer::create(const VkBufferCreateInfo &create_info, device *device,
	                            const VkMemoryPropertyFlags &properties) {
		VkBuffer handle;
		lvk_throw_if_failed_ex(vkCreateBuffer(**device, &create_info,
			                       default_vk_allocation_callbacks, &handle),
		                       "failed to create buffer");
		vk_buffer result(handle, device);
		result.record.size = create_info.size;
		result.memory = result.allocate_memory(properties);
		lvk_throw_if_failed(vkBindBufferMemory(**device, handle, result.memory,
			result.record.offset = 0));
		return result;
	}

	vk_buffer::vk_buffer(vk_buffer &&old) noexcept :
		handle(old.handle), base(old.base), memory(old.memory), record(old.record) {
		old.handle = VK_NULL_HANDLE, old.memory = VK_NULL_HANDLE;
	}

	vk_buffer::~vk_buffer() noexcept {
		if (memory != VK_NULL_HANDLE) {
			vkFreeMemory(**base, memory, default_vk_allocation_callbacks);
		}
		if (handle != VK_NULL_HANDLE) {
			vkDestroyBuffer(**base, handle, default_vk_allocation_callbacks);
		}
	}

	VkMemoryRequirements vk_buffer::memory_requirements() const {
		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(**base, handle, &requirements);
		return requirements;
	}

	device &vk_buffer::parent_device() const { return *base; }

	void *vk_buffer::map(VkMemoryMapFlags flags) {
		if (!record.mapped) {
			lvk_throw_if_failed_ex(vkMapMemory(**base, memory, record.offset, record.size,
				                       flags, &record.mapped),
			                       "failed to map buffer memory");
		}
		return record.mapped;
	}

	void vk_buffer::unmap() {
		if (record.mapped) {
			vkUnmapMemory(**base, memory);
			record.mapped = nullptr;
		}
	}


} // namespace lvk
