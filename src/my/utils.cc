#include "utils.h"

std::string my::remove_string_spaces(std::string_view str) {
	std::string ret(str);
	remove_string_spaces(ret);
	return ret;
}
