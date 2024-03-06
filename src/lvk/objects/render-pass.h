#ifndef LVK_RENDER_PASS_H
#define LVK_RENDER_PASS_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace lvk {

	class render_pass : public vk_object {

		VkRenderPass handle;
		device *base;

		render_pass(const VkRenderPass &handle, lvk::device *base);

	public:
		explicit render_pass(device *device,
		                     const std::vector<VkAttachmentDescription> &attachments = {},
		                     const std::vector<VkSubpassDescription> &subpasses = {},
		                     const std::vector<VkSubpassDependency> &dependencies = {},
		                     const VkRenderPassCreateFlags &flags = 0);

		render_pass(const render_pass &) = delete;

		render_pass(render_pass &&) noexcept;

		~render_pass();

		VkRenderPass operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;
	};

} // lvk

#endif //LVK_RENDER_PASS_H
