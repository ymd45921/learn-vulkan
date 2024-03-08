#include "render-pass.h"
#include "device.h"
#include "../exception.h"
#include "../inlines.h"

namespace lvk {

	render_pass::render_pass(const VkRenderPass &handle, lvk::device *base) :
		handle(handle), base(base) {}

	render_pass::render_pass(device *device,
	                         const std::vector<VkAttachmentDescription> &attachments,
	                         const std::vector<VkSubpassDescription> &subpasses,
	                         const std::vector<VkSubpassDependency> &dependencies,
	                         const VkRenderPassCreateFlags &flags) :
		handle(VK_NULL_HANDLE), base(device) {
		VkRenderPassCreateInfo create_info{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.flags = flags,
			.attachmentCount = static_cast<uint32_t>(attachments.size()),
			.pAttachments = attachments.data(),
			.subpassCount = static_cast<uint32_t>(subpasses.size()),
			.pSubpasses = subpasses.data(),
			.dependencyCount = static_cast<uint32_t>(dependencies.size()),
			.pDependencies = dependencies.data(),
		};
		lvk_throw_if_failed(vkCreateRenderPass(**device, &create_info,
			default_vk_allocation_callbacks, &handle));
	}

	render_pass::render_pass(render_pass &&old) noexcept {
		handle = old.handle, base = old.base;
		handle = VK_NULL_HANDLE;
	}

	render_pass::~render_pass() {
		if (handle != VK_NULL_HANDLE)
			vkDestroyRenderPass(**base, handle, default_vk_allocation_callbacks);
	}

	device & render_pass::parent_device() const { return *base; }

} // lvk
