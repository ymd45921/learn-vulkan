#ifndef LVK_PIPELINE_H
#define LVK_PIPELINE_H
#pragma once

#include "../fwd.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace lvk {

	class pipeline : public vk_object {

		VkPipeline handle;
		device *base;

		pipeline(device *base, VkPipeline handle);

	public:
		pipeline(const pipeline &) = delete;

		pipeline(pipeline &&) noexcept;

		static pipeline create(device *base,
		                       const VkGraphicsPipelineCreateInfo &info,
		                       const VkPipelineCache &cache = VK_NULL_HANDLE);

		// * 似乎 pipeline 的创建都是批量操作的？那么具体要怎么批量呢？
		static std::vector<pipeline> create_pipelines(
			device *base,
			const std::vector<VkGraphicsPipelineCreateInfo> &infos,
			const VkPipelineCache &cache = VK_NULL_HANDLE);

		// 疑似没有 vkDestroyPipelines 函数，所以就先一个个销毁得了
		~pipeline() noexcept;

		VkPipeline operator*() const { return handle; }

		[[nodiscard]] device &parent_device() const { return *base; }

	};

} // lvk

#endif //LVK_PIPELINE_H
