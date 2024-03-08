#include "pipeline.h"
#include "../exception.h"
#include "../inlines.h"
#include "device.h"

namespace lvk {

	pipeline::pipeline(device *base, VkPipeline handle) :
		handle(handle), base(base) {}

	pipeline::pipeline(pipeline &&old) noexcept {
		handle = old.handle, base = old.base;
		old.handle = VK_NULL_HANDLE, old.base = nullptr;
	}

	pipeline pipeline::create(
		device *base, const VkGraphicsPipelineCreateInfo &info,
		const VkPipelineCache &cache) {
		VkPipeline handle;
		lvk_throw_if_failed(vkCreateGraphicsPipelines(
			**base, cache, 1, &info, default_vk_allocation_callbacks, &handle));
		return {base, handle};
	}

	std::vector<pipeline> pipeline::create_pipelines(
		device *base,
		const std::vector<VkGraphicsPipelineCreateInfo> &infos,
		const VkPipelineCache &cache) {
		std::vector<VkPipeline> handles(infos.size());
		lvk_throw_if_failed(vkCreateGraphicsPipelines(
			**base, cache, static_cast<uint32_t>(infos.size()), infos.data(),
			default_vk_allocation_callbacks, handles.data()));
		std::vector<pipeline> result;
		result.reserve(infos.size());
		for (auto &handle : handles) {
			result.push_back({base, handle}); // * 即使在类的上下文内，vector 也无法调用私有成员
		}
		return result;
	}

	pipeline::~pipeline() noexcept {
		if (handle != VK_NULL_HANDLE && base && **base) {
			vkDestroyPipeline(**base, handle, default_vk_allocation_callbacks);
		}
	}


} // lvk
