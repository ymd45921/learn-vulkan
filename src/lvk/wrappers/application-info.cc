#include "application-info.h"

namespace lvk::wrappers {


	application_info application_info::create(const char *app_name,
	                                          const version app_version,
	                                          const char *engine_name,
	                                          const version engine_version,
	                                          const uint32_t api_version) {
		return {{{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = app_name,
			.applicationVersion = app_version,
			.pEngineName = engine_name,
			.engineVersion = engine_version,
			.apiVersion = api_version
		}}};
	}
} // lvk::wrappers
