#include "shader.h"
#include "exception.h"
#include "objects/device.h"
#include "my/fs.h"

namespace lvk {

	shader_stage operator|(shader_stage lhs, shader_stage rhs) {
		return static_cast<shader_stage>(
			static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	void shader::create_shader_module() {
		const VkShaderModuleCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = byte_code.size(),
			.pCode = byte_code.as<uint32_t>(),
		};
		lvk_throw_if_failed_ex(
			vkCreateShaderModule(**base, &create_info, nullptr, &module),
			"failed to create shader module");
	}

	shader::shader(device *device, const shader_stage stage, const unsigned char *code,
	               const size_t size, const char *entry) :
		stage_bit(stage), base(device), entry(entry), byte_code(code, size),
		module(VK_NULL_HANDLE) { create_shader_module(); }

	shader::shader(shader &&old) noexcept {
		stage_bit = old.stage_bit, base = old.base, module = old.module;
		byte_code = std::move(old.byte_code), entry = std::move(old.entry);
		old.module = VK_NULL_HANDLE, base = nullptr;
	}

	shader::shader(device *device, const shader_stage stage, const char *path,
	               const char *entry) :
		stage_bit(stage), base(device), entry(entry), byte_code(my::read_file_sync(path)),
		module(VK_NULL_HANDLE) {}

	shader::~shader() {
		if (module != VK_NULL_HANDLE) { vkDestroyShaderModule(**base, module, nullptr); }
	}

	VkShaderModule shader::shader_module() const { return module; }

	VkPipelineShaderStageCreateInfo shader::stage_info() const {
		return {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = static_cast<VkShaderStageFlagBits>(stage_bit),
			.module = module,
			.pName = entry.c_str(), // ? 或许参数传入指定更好？这样一说连 stage 都很难说了……
			.pSpecializationInfo = nullptr, // todo: 参数传入常量映射表以分支消除？
		};
	}

} // lvk
