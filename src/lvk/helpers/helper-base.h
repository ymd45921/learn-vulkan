#ifndef LVK_HELPER_BASE_H
#define LVK_HELPER_BASE_H
#pragma once

#include <vector>

namespace lvk::helpers {

	struct helper_base {};

	template <typename T>
	const T *data_pointer(const std::vector<T> &vec) {
		return vec.empty() ? nullptr : vec.data();
	}

	template <typename T>
	uint32_t data_count(const std::vector<T> &vec) {
		return static_cast<uint32_t>(vec.size());
	}

} // namespace lvk::helpers

#endif //LVK_HELPER_BASE_H
