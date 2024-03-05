#include "exception.h"
#include "my/utils.h"
#include "utils.h"

constexpr auto lvk_auto_exception_message_prefix = "lvk::vk_auto_exception: ";

lvk::vk_auto_exception::vk_auto_exception(const std::string &message) :
	vk_exception(lvk_auto_exception_message_prefix + message) {}

lvk::vk_auto_exception::vk_auto_exception(const std::string &message, VkResult result) :
	vk_exception(lvk_auto_exception_message_prefix + message + " (" +
				 vk_result_to_string(result) + ")") {}

lvk::vk_auto_exception::vk_auto_exception(const std::string &message, VkResult result,
										  std::string expr, str fn, str file, int line) :
	vk_exception( // NOLINT(*-unnecessary-value-param)
		std::string(lvk_auto_exception_message_prefix) + "Expression \'" +
		my::remove_string_spaces(expr) + "\' throws " + message + " (" +
		vk_result_to_string(result) + ") in function \'" + fn + "\' at " + file +
		", line" + std::to_string(line)) {}
