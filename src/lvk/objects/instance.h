#ifndef LVK_INSTANCE_H
#define LVK_INSTANCE_H
#pragma once

#include "lvk/exception.h"
#include "lvk/fwd.h"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <functional>

#endif // LVK_INSTANCE_H
namespace lvk {

	/**
	 * @brief   Vulkan 实例 RAII 封装
	 * @details 在 Debug 模式下，会自动创建调试信息回调
	 */
	class instance : public vk_object {

		VkInstance handle;

#ifdef LearnVulkan_debug
		VkDebugUtilsMessengerEXT debug_messenger;
#endif

	public:
		explicit instance(
			std::string_view app_name = "Vulkan Application", // todo: 增添 app_version
			std::vector<str> enabled_extensions = {},
			std::vector<str> enabled_layers = {}, unsigned flags = 0);

		instance(const instance &) = delete;

		instance(instance &&) noexcept;

		~instance();

		VkInstance operator*() const { return handle; }

		[[nodiscard]] std::vector<physical_device> get_physical_devices() const;

		[[nodiscard]] physical_device
		pick_physical_device(const std::vector<str> &required_extensions = {}) const;
	};

	/**
	 * @brief   从 Vulkan 实例中动态获取 API 函数指针
	 * @see	 	lvk/lvk.h 中定义的 lvk::proc_loader
	 */
	template <typename pf_vk_api>
	std::function<std::remove_pointer_t<pf_vk_api>>
	proc_loader(const instance &vki, str api_name, bool throw_on_failure = true) {
		return proc_loader<pf_vk_api>(*vki, api_name, throw_on_failure);
	}
	template <typename pf_vk_api>
	std::function<std::remove_pointer_t<pf_vk_api>>
	proc_loader(const instance *vki, str api_name, bool throw_on_failure = true) {
		return proc_loader<pf_vk_api>(**vki, api_name, throw_on_failure);
	}
} // namespace lvk
