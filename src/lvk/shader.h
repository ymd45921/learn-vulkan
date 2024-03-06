#ifndef LVK_SHADER_H
#define LVK_SHADER_H

#include "my/buffer.h"
#include "fwd.h"

#include <string>
#include <vulkan/vulkan.h>

namespace lvk {

	enum class shader_stage {
		vertex = 0x1,
		tessellation_control = 0x2,
		tessellation_evaluation = 0x4,
		geometry = 0x8,
		fragment = 0x10,
		compute = 0x20,
		all_graphics = 0x1f,
		all = 0x7fffffff,
		raygen_khr = 0x100,
		any_hit_khr = 0x200,
		closest_hit_khr = 0x400,
		miss_khr = 0x800,
		intersection_khr = 0x1000,
		callable_khr = 0x2000,
		task_ext = 0x40,
		mesh_ext = 0x80,
		subpass_shading_huawei = 0x4000,
		cluster_culling_huawei = 0x80000,
		flags_max_enum = 0x7fffffff,
		raygen_nv = raygen_khr,
		any_hit_nv = any_hit_khr,
		closest_hit_nv = closest_hit_khr,
		miss_nv = miss_khr,
		intersection_nv = intersection_khr,
		callable_nv = callable_khr,
		task_nv = task_ext,
		mesh_nv = mesh_ext,
	};

	shader_stage operator|(shader_stage lhs, shader_stage rhs);

	class shader {

		shader_stage stage_bit;
		device *base;
		std::string entry;
		my::buffer byte_code {}; // ? 是否一定要持有字节码？

		VkShaderModule module;

		void create_shader_module();

	public:
		shader(device *device, shader_stage stage, const unsigned char *code, size_t size,
		       const char *entry = "main");

		shader(const shader &) = delete; // 应该不需要复制吧？

		shader(shader &&) noexcept;

		template <std::ranges::contiguous_range Buffer,
			typename El = decltype(*std::declval<Buffer>().data())>
			requires std::is_convertible_v<
				decltype(std::declval<Buffer>().data()), const unsigned char *> &&
			std::is_same_v<decltype(std::declval<Buffer>().size()), size_t>
		shader(device *device, const shader_stage stage, const Buffer &buffer,
		       const char *entry = "main") :
			shader(device, stage, buffer.data(), buffer.size() * sizeof(El), entry) {}

		template <typename T>
		shader(device *device, const shader_stage stage,
		       const T *code_begin, const T *code_end, const char *entry = "main") :
			shader(device, stage, code_begin, (code_end - code_begin) * sizeof(T),
			       entry) {}

		shader(device *device, shader_stage stage, const char *path,
		       const char *entry = "main");

		// todo: 输入代码直接编译？

		~shader();

		[[nodiscard]] VkShaderModule shader_module() const;

		[[nodiscard]] VkPipelineShaderStageCreateInfo stage_info() const;
	};

} // lvk

// * 可以重复声明；最后再总的出口文件中定义一下
using lvk::operator|;

#endif //LVK_SHADER_H
