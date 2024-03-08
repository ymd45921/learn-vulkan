#ifndef LVK_WRAPPERS_TRAITS_H
#define LVK_WRAPPERS_TRAITS_H
#pragma once

#include <vulkan/vulkan.h>
#include <ranges>

#include "my/type-traits.h"

namespace lvk::wrappers {

	/**
	 * @brief	包装 Vulkan 结构体的接口
	 * @details 该接口继承自 Vulkan 结构体，提供了一些额外的方法，用于操作 Vulkan 结构体。
	 *			作出约定：任何由该类派生的类都应该是一个标准布局的类，以方便类型转换。
	 * @tparam vk_struct	被包装的 Vulkan 结构体
	 */
	template <typename vk_struct>
	struct wrapper_interface : vk_struct {

		using type = vk_struct;
		using super [[maybe_unused]] = wrapper_interface;

		vk_struct operator*() const { return *this; }

		// ! dangerously overload unary operator &!!!
		// vk_struct *operator&() const { return this; }

		type &unwrap() & { return *this; }

		type unwrap() && { return std::move(*this); }

		type *address() const { return this; }

		// ReSharper disable once CppNonExplicitConversionOperator
		operator type() & { return *this; } // NOLINT(*-explicit-constructor)
		// ReSharper disable once CppNonExplicitConversionOperator // 应该无意义？
		operator type() && { return std::move(*this); } // NOLINT(*-explicit-constructor)
	};

	/**
	 * @brief	判断一个 Vulkan 内置结构体类型是否是 wrappable 的
	 * @details	作出约定，任何派生自 wrapper_interface 的类都是 wrappable 的。
	 *			因此，实现这些类时应当手动特化该模版。
	 * @tparam T	要判断的 Vulkan 内置结构体类型
	 */
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

	template <typename T, bool = wrapper<T>, bool = wrappable<T>>
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
		requires my::is_single_tparam_template_v<R> && (!std::is_rvalue_reference_v<R&&>)
	my::replace_only_tparam_t<R, wrap_inner_t<T>> &unsafe_wrap(R &&range) {
		return *reinterpret_cast<my::replace_only_tparam_t<R, wrap_inner_t<T>> *>(&range);
	} // 在 wrap 之前需要先使得 range 成为具名符号

	template <std::ranges::contiguous_range R,
		wrapper T = my::is_single_tparam_template_t<R>>
	requires my::is_single_tparam_template_v<R> && (!std::is_lvalue_reference_v<R&&>)
	my::replace_only_tparam_t<R, wrap_inner_t<T>> unsafe_wrap(R &&range) {
		return std::move(*reinterpret_cast<my::replace_only_tparam_t<R, wrap_inner_t<T>> *>(&range));
	} // 虽然不可避免的会发生一次析构/构造，但是至少 std::vector 是可以被安全移动的。

} // namespace lvk

#endif //LVK_WRAPPERS_TRAITS_H
