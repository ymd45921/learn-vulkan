#include "surface-capabilities.h"

#include <algorithm>

namespace lvk::wrap {

	uint32_t surface_capabilities::auto_image_count() const {
		return std::min(minImageCount + 1, maxImageCount);
	}

} // namespace lvk::wrap
