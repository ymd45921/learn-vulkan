#ifndef LVK_WRAP_TRAITS_H
#define LVK_WRAP_TRAITS_H
#pragma once

#include <vulkan/vulkan.h>

namespace lvk::wrap {

	template <typename vk_struct>
	struct wrapper_interface : vk_struct {

		using type = vk_struct;
		using super = wrapper_interface<vk_struct>;

		// ? 是否需要构造器来显式转发？实现构造器会屏蔽 record 构造器

		vk_struct operator*() const { return *this; }
	};

	template <typename T>
	struct wrapper_traits {


	};

} // namespace lvk::wrap

namespace lvk {

	template <typename T>
	concept wrapper = std::is_base_of_v<wrap::wrapper_interface<T>, T>;

} // namespace lvk

#endif //LVK_WRAP_TRAITS_H
