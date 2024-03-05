#include "lvk/wrappers/surface-capabilities.h"

void test_type() {

	static_assert(std::is_same_v<VkSurfaceCapabilitiesKHR,
		decltype(*std::declval<lvk::wrap::surface_capabilities>())>);

}