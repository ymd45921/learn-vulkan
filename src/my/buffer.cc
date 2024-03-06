#include "buffer.h"

namespace my {

	buffer::operator vector() { return *this; }

	buffer::operator vector&() { return *this; }

	buffer::operator const vector&() const { return *this; }

	buffer::operator vector&&() noexcept { return std::move(*this); }

} // my