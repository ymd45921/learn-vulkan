#include "pipeline-create-helper.h"


namespace lvk::helpers {

	pipeline_create_helper
	&pipeline_create_helper::setup_default_viewport(
		const VkExtent2D &extent) {
		viewports.push_back({
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(extent.width),
			.height = static_cast<float>(extent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f});
		scissors.push_back({.offset = {0, 0}, .extent = extent});
		return *this;
	}

	VkPipelineVertexInputStateCreateInfo
	pipeline_create_helper::get_vertex_input_state() const {
		return {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = data_count(vertex_input_binding_descs),
			.pVertexBindingDescriptions = data_pointer(vertex_input_binding_descs),
			.vertexAttributeDescriptionCount = data_count(vertex_input_attr_descs),
			.pVertexAttributeDescriptions = data_pointer(vertex_input_attr_descs),
		};
	}

	VkPipelineViewportStateCreateInfo
	pipeline_create_helper::get_viewport_state() const {
		return {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = data_count(viewports),
			.pViewports = data_pointer(viewports),
			.scissorCount = data_count(scissors),
			.pScissors = data_pointer(scissors),
		};
	}

	VkPipelineColorBlendStateCreateInfo
	pipeline_create_helper::get_color_blend_state() const {
		return {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE, // 不启用逻辑操作（即使用算术运算而非逻辑运算进行颜色混合）
			.attachmentCount = data_count(color_blend_attachments),
			.pAttachments = data_pointer(color_blend_attachments),
		};
	}

	VkPipelineMultisampleStateCreateInfo
	pipeline_create_helper::get_multisample_state(
		const VkSampleCountFlagBits sample_count) {
		return {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = sample_count,
			.sampleShadingEnable = VK_FALSE, // 不启用采样着色
		};
	}


} // namespace lvk::helpers
