#ifndef LVK_H
#define LVK_H
#pragma once

#include "exception.h"
#include "fwd.h"
#include "objects/instance.h"
#include "objects/physical-device.h"

#include <functional>
#include <string>

namespace lvk {

	/**
	 * @brief   从 Vulkan 实例中选择物理设备
	 * @tparam score_fun 	评分函数类型
	 * @param vki			Vulkan 实例
	 * @param score  		评分函数
	 * @return 	评分最高的物理设备
	 */
	template <typename score_fun>
		requires std::invocable<score_fun, physical_device> &&
		std::totally_ordered<std::invoke_result_t<score_fun, physical_device>>
	physical_device pick_physical_device(const instance &vki, score_fun score) {
		auto devices = vki.get_physical_devices();
		auto result = std::ranges::max_element(devices, score);
		if (result == devices.end()) {
			throw vk_exception("lvk::pick_physical_device: no suitable "
							   "physical device found.");
		}
		return *result;
	}


	/**
	 * @brief   用于从 Vulkan 实例中加载 Vulkan API 函数指针
	 * @see	 	lvk/objects/instance.h 中有对于包装类的重载
	 * @tparam pf_vk_api        Vulkan API 函数指针类型，往往以 PFN_ 开头
	 * @param vki               Vulkan 实例句柄，被传递给 vkGetInstanceProcAddr
	 * @param api_name          Vulkan API 函数名，由宏 #api_name 自动填写
	 * @param throw_on_failure  是否在加载失败时抛出异常，为 false
	 * 会返回空函数对象
	 * @return                  目标 Vulkan API 函数指针
	 */
	template <typename pf_vk_api>
	std::function<std::remove_pointer_t<pf_vk_api>>
	proc_loader(VkInstance vki, str api_name, bool throw_on_failure = true) {
		auto proc = vkGetInstanceProcAddr(vki, api_name);
		if (throw_on_failure && !proc) {
			throw vk_exception(std::string("lvk::proc_loader: failed to load proc \'") +
							   api_name + "\'.");
		} else
			return reinterpret_cast<pf_vk_api>(proc);
	}

} // namespace lvk

// todo: 使用 Volk 代替手动加载 Vulkan API；
// ! Mac 上的 FindVulkan 找不到 Volk
#define lvk_load_vulkan_api(vki, api_name)                                               \
	lvk::proc_loader<PFN_##api_name>(vki, #api_name)

#endif // LVK_H
