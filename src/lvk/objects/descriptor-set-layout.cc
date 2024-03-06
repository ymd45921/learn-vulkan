#include "descriptor-set-layout.h"
#include "../exception.h"
#include "device.h"
#include "lvk/inlines.h"

namespace lvk {

	descriptor_set_layout::descriptor_set_layout(
		const VkDescriptorSetLayoutCreateInfo &create_info, device *base) :
		base(base),
		handle(VK_NULL_HANDLE) {
		lvk_throw_if_failed_ex(
			vkCreateDescriptorSetLayout(**base, &create_info,
										default_vk_allocation_callbacks, &handle),
			"failed to create descriptor set layout");
	}

	descriptor_set_layout::descriptor_set_layout(descriptor_set_layout &&old) noexcept {
		handle = old.handle, base = old.base;
		old.handle = VK_NULL_HANDLE;
	}

	descriptor_set_layout::~descriptor_set_layout() noexcept {
		if (handle != VK_NULL_HANDLE) {
			vkDestroyDescriptorSetLayout(**base, handle, default_vk_allocation_callbacks);
		}
	}

	device & descriptor_set_layout::parent_device() const { return *base; }


} // namespace lvk
