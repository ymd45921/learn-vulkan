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

} // namespace lvk


#endif // LVK_FWD_H
