#ifndef MY_UTILS_H
#define MY_UTILS_H
#pragma once

#include <string>

namespace my {

	inline std::string remove_string_spaces(std::string &str) {
		std::erase_if(str, [](char c) { return std::isspace(c); });
		return str;
	}

	std::string remove_string_spaces(std::string_view str);




}

#endif //MY_UTILS_H
