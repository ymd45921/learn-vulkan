#ifndef MY_BUFFER_H
#define MY_BUFFER_H

#include "fwd.h"

#include <vector>
#include <span>

namespace my {

	struct buffer : std::vector<byte> {

		using type = std::vector<byte>;
		using view = std::span<byte>;

		template <typename... Args>
			requires std::constructible_from<std::vector<byte>, Args...>
		explicit buffer(Args &&... args) :
			std::vector<byte>(std::forward<Args>(args)...) {}

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
