#include "buffer.h"

namespace my {

	buffer::operator vector<char>() { return *this; }

	buffer::operator vector<char>&() { return *this; }

	buffer::operator const vector<char>&() const { return *this; }

	buffer::operator vector<char>&&() noexcept { return std::move(*this); }

} // my