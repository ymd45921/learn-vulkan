#ifndef LVK_SURFACE_CAPABILITIES_H
#define LVK_SURFACE_CAPABILITIES_H
#pragma once

#include "../fwd.h"
#include "traits.h"

#include <vulkan/vulkan.h>

namespace lvk::wrap {

	struct surface_capabilities : wrapper_interface<VkSurfaceCapabilitiesKHR> {

		[[nodiscard]] inline uint32_t auto_image_count() const;
	};

} // namespace lvk::wrap

#endif //LVK_SURFACE_CAPABILITIES_H
