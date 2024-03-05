#ifndef LVK_EXCEPTION_H
#define LVK_EXCEPTION_H
#pragma once

#include <stdexcept>

#include "fwd.h"
#include "vulkan/vulkan.h"

namespace lvk {

	using vk_exception = std::runtime_error;

	/**
	 * @brief   被宏 lvk_throw_if_failed 内部调用的异常类
	 */
	class vk_auto_exception final : public vk_exception {

	public:
		static constexpr auto no_message = "no message.";

		explicit vk_auto_exception(const std::string &message);

		vk_auto_exception(const std::string &message, VkResult result);

		vk_auto_exception(const std::string &message, VkResult result, std::string expr,
						  str fn, str file, int line);
	};

} // namespace lvk

#define lvk_throw_if_failed_ex(action, message)                                          \
	{                                                                                    \
		VkResult __result = (action);                                                    \
		if (__result != VK_SUCCESS) {                                                    \
			throw lvk::vk_auto_exception(message, __result, #action, __func__, __FILE__, \
										 __LINE__);                                      \
		}                                                                                \
	}                                                                                    \
	void(0)

#define lvk_throw_if_failed(action)                                                      \
	lvk_throw_if_failed_ex(action, lvk::vk_auto_exception::no_message)

#endif // LVK_EXCEPTION_H
