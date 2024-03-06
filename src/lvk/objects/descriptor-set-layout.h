#ifndef DESCRIPTOR_SET_LAYOUT_H
#define DESCRIPTOR_SET_LAYOUT_H
#pragma once

#include "../fwd.h"

#include <ranges>
#include <vulkan/vulkan.h>

namespace lvk {

	class descriptor_set_layout : public vk_object {

		VkDescriptorSetLayout handle;
		device *base;

		descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info,
							  device *base);

	public:
		descriptor_set_layout(device *base, std::ranges::contiguous_range auto &&bindings,
							  VkDescriptorSetLayoutCreateFlags flags = 0);

		descriptor_set_layout(const descriptor_set_layout &) = delete;

		descriptor_set_layout(descriptor_set_layout &&) noexcept;

		~descriptor_set_layout() noexcept;

		VkDescriptorSetLayout operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const;
	};

	descriptor_set_layout::descriptor_set_layout(
		device *base, std::ranges::contiguous_range auto &&bindings,
		VkDescriptorSetLayoutCreateFlags flags) :
		descriptor_set_layout(
			{.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			 .pNext = nullptr,
			 .flags = flags,
			 .bindingCount = static_cast<uint32_t>(std::ranges::size(bindings)),
			 .pBindings = std::ranges::data(bindings)},
			base) {}

} // namespace lvk

#endif // DESCRIPTOR_SET_LAYOUT_H
