#ifndef LVK_WRAP_TRAITS_H
#define LVK_WRAP_TRAITS_H
#pragma once

#include <vulkan/vulkan.h>
#include <ranges>

#include "my/type-traits.h"

namespace lvk::wrappers {

	template <typename vk_struct>
	struct wrapper_interface : vk_struct {

		using type = vk_struct;
		using super [[maybe_unused]] = wrapper_interface;

		// ? 是否需要构造器来显式转发？实现构造器会屏蔽 record 构造器

		vk_struct operator*() const { return *this; }

		type &unwrap() const { return *this; }
	};

	template <typename T>
	struct is_wrappable : std::false_type {
		using wrapped_type = void;
		using inner_type = T;
	}; // todo: 当一个类型时 wrappable 的时候，自动让 cv 版本也是 wrappable 的

	template <typename T> requires std::is_base_of_v<wrapper_interface<typename T::type>, T>
	struct is_wrappable<T> : std::false_type {
		using wrapped_type = void;
		using inner_type = T;
	};

	template <typename T>
	constexpr auto is_wrappable_v = is_wrappable<T>::value;

	template <typename T>
	using is_wrappable_t = typename is_wrappable<T>::wrapped_type;

} // namespace lvk::wrappers

namespace lvk {

	template <typename T>
	concept wrapper = std::is_base_of_v<wrappers::wrapper_interface<typename T::type>, T>;

	template <typename T>
	concept wrappable = wrappers::is_wrappable_v<T>;

	template <typename T, bool isWrapper = wrapper<T>, bool isWrappable = wrappable<T>>
	struct wrapper_traits {};

	template <typename T>
	struct wrapper_traits<T, true, false> {
		using handle_t = typename T::type;
		using record_t = T;
	};

	template <typename T>
	struct wrapper_traits<T, false, true> {
		using handle_t = T;
		using record_t = wrappers::is_wrappable_t<T>;
	};

	template <typename T>
	using wrap_inner_t = typename wrapper_traits<T>::handle_t;

	template <typename T>
	using wrap_outer_t = typename wrapper_traits<T>::record_t;

	template <wrapper T>
	wrap_inner_t<T> unwrap(const T &wrapper) { return wrapper.unwrap(); }

	// * 万能引用模版优先级还是比想象中高：
	// * 传入连续容器时，该模版优先级和模版 2 相同，引发二义。
	// ? 这种情况下面的具体性不是明显更高吗……？
	// template <typename T>
	// T wrap(T &&transfer) { return std::forward<T>(transfer); }

	template <wrappable T>
	wrap_outer_t<T> wrap(T &&handle) { return handle; }

	template <std::ranges::contiguous_range R,
		wrappable T = my::is_single_tparam_template_t<R>>
		requires my::is_single_tparam_template_v<R>
	my::replace_only_tparam_t<R, wrap_outer_t<T>> wrap(R &&range) {
		auto result = range | std::views::transform(
			[](auto &&handle) { return wrap<T>(std::forward<decltype(handle)>(handle)); });
		return {result.begin(), result.end()};
	}

	template <std::ranges::contiguous_range R,
		wrapper T = my::is_single_tparam_template_t<R>>
		requires my::is_single_tparam_template_v<R>
	my::replace_only_tparam_t<R, wrap_inner_t<T>> &unsafe_unwrap(R &&range) {
		static_assert(!std::is_rvalue_reference_v<R&&>,
		    "unsafe_unwrap cannot be used on rvalue reference.");
		return *reinterpret_cast<my::replace_only_tparam_t<R, wrap_inner_t<T>> *>(&range);
	}

} // namespace lvk

#endif //LVK_WRAP_TRAITS_H
