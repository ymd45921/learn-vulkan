#ifndef LVK_APPLICATION_INFO_H
#define LVK_APPLICATION_INFO_H
#pragma once

#include "lvk/fwd.h"
#include "lvk/wrappers/traits.h"
#include "lvk/utils.h"

#include <vulkan/vulkan.h>

namespace lvk {
	constexpr version operator ""_vk_version(const char *str, size_t len) noexcept;
	template <std::integral T> // ? 为什么 IDE 报错匹配不到？
	constexpr version operator ""_vk_version(T major) noexcept;
	template <std::floating_point T> // ? 为什么 IDE 报错匹配不到？
	constexpr version operator ""_vk_version(T major_and_minor) noexcept;
}

namespace lvk::wrappers {

	template <>
	struct is_wrappable<VkApplicationInfo> : std::true_type {
		using wrapped_type = application_info;
		using inner_type = VkApplicationInfo;
	};

	struct application_info
		: wrapper_interface<VkApplicationInfo> {

		static application_info create(
			const char *app_name = CMAKE_PROJECT_NAME,
			version app_version = operator""_vk_version(
				CMAKE_PROJECT_VERSION, std::strlen(CMAKE_PROJECT_VERSION)),
			const char *engine_name = "No Engine",
			version engine_version = "1.0"_vk_version,
			uint32_t api_version = VK_API_VERSION_1_0);
	};

} // namespace lvk::wrappers

namespace lvk {

	constexpr version
	operator ""_vk_version(const char *str, const size_t len) noexcept {
		static_assert(is_vk_version(str, len), "Invalid version string");
		uint32_t version[3] {}, p = 0;
		for (size_t i = 0; i < len; ++i) {
			if (str[i] == '.') { ++p; } else {
				version[p] = version[p] * 10 + (str[i] - '0');
			}
		}
		return VK_MAKE_VERSION(version[0], version[1], version[2]);
	}

	template <std::integral T>
	constexpr version
	operator ""_vk_version(const T major) noexcept {
		return VK_MAKE_VERSION(major, 0, 0);
	}

	template <std::floating_point T>
	constexpr version
	operator ""_vk_version(const T major_and_minor) noexcept {
		const auto major = static_cast<uint32_t>(major_and_minor);
		const auto minor = static_cast<uint32_t>((major_and_minor - major) * 10);
		return VK_MAKE_VERSION(major, minor, 0);
	} // ?
}

using lvk::operator""_vk_version;

#endif //LVK_APPLICATION_INFO_H
