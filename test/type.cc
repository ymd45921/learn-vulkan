#include "lvk/wrappers/surface-capabilities.h"
#include "lvk/objects/physical-device.h"

#include "my/type-traits.h"

#include <vector>
#include <queue>

void test_type() {

	static_assert(std::is_same_v<VkSurfaceCapabilitiesKHR,
		decltype(*std::declval<lvk::wrappers::surface_capabilities>())>);

	static_assert(lvk::wrapper<lvk::wrappers::surface_capabilities>);

	static_assert(lvk::wrappable<VkSurfaceCapabilitiesKHR>);

	static_assert(!lvk::wrappable<VkSurfaceCapabilitiesKHR &>);

	static_assert(std::is_same_v<lvk::wrap_outer_t<VkSurfaceCapabilitiesKHR>,
	    lvk::wrappers::surface_capabilities>);

	static_assert(std::is_same_v<VkSurfaceCapabilitiesKHR,
		lvk::wrap_inner_t<lvk::wrappers::surface_capabilities>>);

	static_assert(std::is_same_v<std::vector<long>,
	    my::replace_only_tparam_t<std::vector<int>, long>>);

	static_assert(std::is_same_v<std::vector<long>,
		my::replace_only_tparam_t<std::vector<int>, long>>);

	static_assert(std::is_same_v<std::vector<lvk::wrappers::surface_capabilities>,
		decltype(lvk::wrap(std::declval<std::vector<VkSurfaceCapabilitiesKHR>>()))>);
}