#include "buffer.h"

namespace my {

	void * buffer::raw() noexcept { return data(); }

	buffer::view buffer::borrow() noexcept { return {data(), size()}; }

	buffer::operator vector() { return *this; }

	buffer::operator vector&() { return *this; }

	buffer::operator const vector&() const { return *this; }

	buffer::operator vector&&() noexcept { return std::move(*this); }

} // my