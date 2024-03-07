#include "lvk/utils.h"

void test_utils() {

	// test is_vk_version
	static_assert(lvk::is_vk_version("1.0.0"));
	static_assert(lvk::is_vk_version("2233."));
	static_assert(lvk::is_vk_version("80.443"));
	static_assert(lvk::is_vk_version("20.1."));
	static_assert(lvk::is_vk_version("114514.1919810.810893"));
	static_assert(lvk::is_vk_version("v45921.7890"));
	static_assert(lvk::is_vk_version("v1.0.0"));
	static_assert(lvk::is_vk_version("v"));
	static_assert(!lvk::is_vk_version("nmsl"));
	static_assert(!lvk::is_vk_version("1.0.0-alpha"));
	static_assert(!lvk::is_vk_version("MIT6.828"));
	static_assert(!lvk::is_vk_version("ISBN-13: 978-4-87311-690-4"));
	static_assert(!lvk::is_vk_version("vme50"));

}