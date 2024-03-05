#ifndef LVK_FWD_H
#define LVK_FWD_H
#pragma once

#include <concepts>

namespace lvk {

	using str = const char *;

	enum class exit_code : int { success = 0, failure = 1 };

	struct vk_object {};
	class glfw_window;
	class instance;
	class physical_device;
	class surface;
	class device;
	class descriptor_set_layout;
	class swapchain;

	class vk_image2d;
	class vk_buffer;

	namespace wrappers {

		struct surface_capabilities;

	} // namespace wrappers

} // namespace lvk


#endif // LVK_FWD_H
