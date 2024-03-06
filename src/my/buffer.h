#ifndef MY_BUFFER_H
#define MY_BUFFER_H

#include "fwd.h"

#include <vector>
#include <span>

namespace my {

	struct buffer : std::vector<char> {

		template <typename... Args>
			requires std::constructible_from<std::vector<char>, Args...>
		explicit buffer(Args &&... args) :
			std::vector<char>(std::forward<Args>(args)...) {}

		using type = std::vector<char>;
		using view = std::span<char>;

		// ReSharper disable once CppNonExplicitConversionOperator
		operator type(); // NOLINT(*-explicit-constructor)
		// ReSharper disable once CppNonExplicitConversionOperator
		operator type &(); // NOLINT(*-explicit-constructor)
		// ReSharper disable once CppNonExplicitConversionOperator
		operator const type &() const; // NOLINT(*-explicit-constructor)
		// ReSharper disable once CppNonExplicitConversionOperator
		operator type &&() noexcept; // NOLINT(*-explicit-constructor)
	};

} // my

#endif //MY_BUFFER_H
