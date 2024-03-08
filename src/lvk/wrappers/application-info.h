#ifndef LVK_APPLICATION_INFO_H
#define LVK_APPLICATION_INFO_H
#pragma once

#include "lvk/fwd.h"
#include "lvk/wrappers/traits.h"
#include "lvk/utils.h"

#include <vulkan/vulkan.h>

namespace lvk {
	constexpr version operator ""_vk_version(const char *str, size_t len) noexcept;

	constexpr version operator ""_vk_version(unsigned long long major) noexcept;

	constexpr version operator ""_vk_version(long double major_and_minor) noexcept;
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
			version app_version = CMAKE_PROJECT_VERSION""_vk_version,
			const char *engine_name = "No Engine",
			version engine_version = "1.0"_vk_version, // ? 这不是常量表达式？难道默认参数不是编译期常量吗？
			uint32_t api_version = VK_API_VERSION_1_0);
	};

} // namespace lvk::wrappers

namespace lvk {

	constexpr version
	operator ""_vk_version(const char *str, const size_t len) noexcept {
		// ? 有没有 constexpr 函数中只在编译期进行 static_assert，运行时什么都不做的方案？
		// static_assert(is_vk_version(str, len), "Invalid version string");
		uint32_t version[3] {}, p = 0;
		for (size_t i = 0; i < len; ++i) {
			if (str[i] == '.') { ++p; } else {
				version[p] = version[p] * 10 + (str[i] - '0');
			}
		}
		return VK_MAKE_VERSION(version[0], version[1], version[2]);
	}

	constexpr version
	operator ""_vk_version(const unsigned long long major) noexcept {
		return VK_MAKE_VERSION(major, 0, 0);
	}

	constexpr version
	operator ""_vk_version(const long double major_and_minor) noexcept {
		const auto major = static_cast<uint32_t>(major_and_minor);
		const auto minor = static_cast<uint32_t>((major_and_minor - major) * 10);
		return VK_MAKE_VERSION(major, minor, 0);
	} // ?
}

using lvk::operator""_vk_version;

#endif //LVK_APPLICATION_INFO_H
