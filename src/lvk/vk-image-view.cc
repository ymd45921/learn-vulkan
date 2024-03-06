#include "vk-image-view.h"
#include "exception.h"
#include "inlines.h"
#include "objects/device.h"

namespace lvk {

	vk_image_view::vk_image_view(const VkImageView &handle, device *base)
		: handle(handle), base(base) { }

	vk_image_view vk_image_view::create(device *device,
		const VkImageViewCreateInfo &create_info) {
		VkImageView handle;
		lvk_throw_if_failed_ex(vkCreateImageView(**device, &create_info, default_vk_allocation_callbacks, &handle),
		                       "failed to create image view");
		return {handle, device};
	}

	device & vk_image_view::parent_device() const { return *base; }



} // lvk