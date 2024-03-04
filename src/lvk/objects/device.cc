#include "device.h"

#include "../inlines.h"

namespace lvk {

	device::device(const VkDevice &handle, lvk::physical_device *base) : handle(handle), base(base) {}

	device::~device() { vkDestroyDevice(handle, default_vk_allocation_callbacks); }

	physical_device &device::physical_device() const { return *base; }

	VkQueue device::get_queue(uint32_t queue_family_index, uint32_t queue_index) const {
		VkQueue result;
		vkGetDeviceQueue(handle, queue_family_index, queue_index, &result);
		return result;
	}


} // lvk