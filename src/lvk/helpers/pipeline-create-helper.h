#ifndef LVK_PIPELINE_CREATE_HELPER_H
#define LVK_PIPELINE_CREATE_HELPER_H
#pragma once

#include "lvk/fwd.h"
#include "helper-base.h"

#include <vulkan/vulkan.h>
#include <ranges>

namespace lvk::helpers {

	/**
	 * @brief	用于辅助创建管线，免去填写一些重复的配置
	 * @warning	感觉有点定位不清意义不明啊…… 只是临时措施罢了
	 */
	struct pipeline_create_helper : helper_base {

		// todo: 重构这些默认值成 builder 模式？
		/* ================================== 默认值 ================================== */

		// * 默认图元装配配置：
		// 三角形列表，不启用图元重启
		static constexpr VkPipelineInputAssemblyStateCreateInfo
		default_input_assembly_state = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // 图元三角形列表
			.primitiveRestartEnable = VK_FALSE, // 不启用图元重启（输入不循环）
		};

		// * 默认光栅化配置：
		// 不启用深度裁剪、片段丢弃、深度偏移；多边形填充模式，逆时针为正面；开启背面剔除
		static constexpr VkPipelineRasterizationStateCreateInfo
		default_rasterization_state = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE, // 不启用深度裁剪
			.rasterizerDiscardEnable = VK_FALSE, // 不启用片段丢弃
			.polygonMode = VK_POLYGON_MODE_FILL, // 多边形填充模式
			.cullMode = VK_CULL_MODE_BACK_BIT, // 背面剔除
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, // 逆时针为正面
			.depthBiasEnable = VK_FALSE, // 不启用深度偏移
			.lineWidth = 1.0f, // 线宽
		};

		// * 默认深度模板测试配置：
		// 启用深度测试、深度写入；深度比较操作为小于；不启用深度范围测试、模板测试
		static constexpr VkPipelineDepthStencilStateCreateInfo
		default_depth_stencil_state = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthTestEnable = VK_TRUE, // 启用深度测试
			.depthWriteEnable = VK_TRUE, // 启用深度写入
			.depthCompareOp = VK_COMPARE_OP_LESS, // 深度比较操作
			.depthBoundsTestEnable = VK_FALSE, // 不启用深度范围测试
			.stencilTestEnable = VK_FALSE, // 不启用模板测试
		};

		// * 默认颜色混合附加：
		// 不启用颜色混合；颜色混合操作为加法混合；颜色混合因子为 1.0
		static constexpr VkPipelineColorBlendAttachmentState
		default_color_blend_attachment = {
			.blendEnable = VK_FALSE, // 不启用颜色混合
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, // 写入所有颜色通道
		};


		/* ================================= 成员变量 ================================= */

		std::vector<VkPipelineShaderStageCreateInfo> shader_stages; // ? 不持有 shader
		std::vector<VkVertexInputBindingDescription> vertex_input_binding_descs; // todo: 需要 helper
		std::vector<VkVertexInputAttributeDescription> vertex_input_attr_descs;
		std::vector<VkViewport> viewports;
		std::vector<VkRect2D> scissors;
		std::vector<VkPipelineColorBlendAttachmentState> color_blend_attachments =
			{default_color_blend_attachment};
		std::vector<VkDynamicState> dynamic_states;


		/* =================================== 方法 =================================== */

		template <std::ranges::forward_range ShaderList>
		pipeline_create_helper &add_shader_stages(const ShaderList &shaders) {
			for (const auto &shader : shaders) {
				shader_stages.push_back(shader->get_stage_info());
			}
			return *this;
		}

		pipeline_create_helper &setup_default_viewport(const VkExtent2D &extent);

		[[nodiscard]] VkPipelineVertexInputStateCreateInfo get_vertex_input_state() const;

		[[nodiscard]] VkPipelineViewportStateCreateInfo get_viewport_state() const;

		[[nodiscard]] VkPipelineColorBlendStateCreateInfo get_color_blend_state() const;

		// [[nodiscard]] VkPipelineDynamicStateCreateInfo get_dynamic_state() const;

		// [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo get_input_assembly_state() const;

		// [[nodiscard]] VkPipelineRasterizationStateCreateInfo get_rasterization_state() const;

		// [[nodiscard]] VkPipelineDepthStencilStateCreateInfo get_depth_stencil_state() const;

		[[nodiscard]] static VkPipelineMultisampleStateCreateInfo
		get_multisample_state(VkSampleCountFlagBits sample_count);
	};

} // namespace lvk::helpers

#endif //LVK_PIPELINE_CREATE_HELPER_H
