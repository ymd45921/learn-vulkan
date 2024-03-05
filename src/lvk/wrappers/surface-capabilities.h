#ifndef LVK_SURFACE_CAPABILITIES_H
#define LVK_SURFACE_CAPABILITIES_H
#pragma once

#include "../fwd.h"
#include "traits.h"

#include <vulkan/vulkan.h>

namespace lvk::wrappers {

	template <>
	struct is_wrappable<VkSurfaceCapabilitiesKHR> : std::true_type {
		using wrapped_type = surface_capabilities;
		using inner_type = VkSurfaceCapabilitiesKHR;
	};

	struct surface_capabilities
		: wrapper_interface<VkSurfaceCapabilitiesKHR> {

		[[nodiscard]] inline uint32_t auto_image_count() const;
	};

} // namespace lvk::wrappers

#endif //LVK_SURFACE_CAPABILITIES_H
