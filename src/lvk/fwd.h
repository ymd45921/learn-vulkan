#ifndef LVK_FWD_H
#define LVK_FWD_H
#pragma once

#include <concepts>
#include <cstdint>

namespace lvk {

	using str = const char *;
	using version = uint32_t;

	enum class exit_code : int { success = 0, failure = 1 };

	struct vk_object {};
	class glfw_window;
	class instance;
	class physical_device;
	class surface;
	class device;
	class descriptor_set_layout;
	class swapchain;
	class render_pass;
	class pipeline;

	class vk_image2d;
	class vk_buffer;
	class vk_image_view;
	enum class shader_stage; // ? ok吗
	class shader;

	namespace wrappers {

		struct surface_capabilities;
		struct application_info;

	} // namespace wrappers

	namespace helpers {

		struct pipeline_create_helper;

	} // namespace helpers

} // namespace lvk


#endif // LVK_FWD_H
