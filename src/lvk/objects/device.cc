#include "device.h"
#include "physical-device.h"
#include "../inlines.h"
#include "lvk/exception.h"

namespace lvk {

	device::device(const VkDevice &handle, lvk::physical_device *base) :
		handle(handle), base(base) {}

	device device::create(lvk::physical_device *physical_device,
		const VkDeviceCreateInfo &create_info) {
		VkDevice handle;
		lvk_throw_if_failed_ex(
			vkCreateDevice(**physical_device, &create_info, default_vk_allocation_callbacks, &handle),
			"failed to create logical device");
		return {handle, physical_device};
	}

	device::device(device &&old) noexcept {
		handle = old.handle, base = old.base;
		old.handle = VK_NULL_HANDLE;
	}

	device::~device() {
		if (handle != VK_NULL_HANDLE)
			vkDestroyDevice(handle, default_vk_allocation_callbacks);
	}

	physical_device &device::physical_device() const { return *base; }

	VkQueue device::get_queue(uint32_t queue_family_index, uint32_t queue_index) const {
		VkQueue result;
		vkGetDeviceQueue(handle, queue_family_index, queue_index, &result);
		return result;
	}


} // namespace lvk
