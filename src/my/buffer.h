#ifndef MY_BUFFER_H
#define MY_BUFFER_H

#include "fwd.h"

#include <vector>
#include <span>
#include <concepts>

namespace my {

	struct buffer : std::vector<byte> {

		using type = std::vector<byte>;
		using view = std::span<byte>;

		template <typename... Args>
			requires std::constructible_from<std::vector<byte>, Args...>
		explicit buffer(Args &&... args) :
			std::vector<byte>(std::forward<Args>(args)...) {}

		template <std::integral T>
		buffer(const T *p, const size_t size) :
			type(reinterpret_cast<const unsigned char *>(p),
			     reinterpret_cast<const unsigned char *>(p) + size * sizeof(T)) {}

		void *raw() noexcept;

		template <std::integral T>
		T *as() noexcept { return reinterpret_cast<T *>(data()); }

		view borrow() noexcept;

		// todo: to_string()

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
