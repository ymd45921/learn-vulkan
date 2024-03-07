set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(CMAKE_EXECUTABLE_PATH "${CMAKE_COMMAND}" DIRECTORY)
string(REGEX REPLACE "bin$" "share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" CMAKE_BUILTIN_MODULE_PATH "${CMAKE_EXECUTABLE_PATH}")
if (NOT EXISTS "${CMAKE_BUILTIN_MODULE_PATH}")
	string(REGEX REPLACE "bin$" "share/cmake/Modules" CMAKE_BUILTIN_MODULE_PATH "${CMAKE_EXECUTABLE_PATH}")
endif ()
add_definitions(-DCMAKE_PROJECT_NAME="${CMAKE_PROJECT_NAME}"
	-DCMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")
if (UNIX AND NOT APPLE)
	set(LINUX ON)
	add_definitions(-DLINUX)
	log_info("Current platform: ${ANSI_BOLD}Linux${ANSI_RESET}")
elseif (APPLE)
	add_definitions(-DAPPLE)
	log_info("Current platform: ${ANSI_BOLD}MacOS${ANSI_RESET}")
elseif (WIN32)
	add_definitions(-DWIN32)
	log_info("Current platform: ${ANSI_BOLD}Windows${ANSI_RESET}")
endif ()
log_info("Using C++ compiler: ${ANSI_BOLD}${CMAKE_CXX_COMPILER_ID}${ANSI_RESET} (${CMAKE_CXX_COMPILER})")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG -D_DEBUG -D${CMAKE_PROJECT_NAME}_DEBUG -D${CMAKE_PROJECT_NAME}_Debug -D${CMAKE_PROJECT_NAME}_debug)
else ()
	add_definitions(-DNDEBUG -D${CMAKE_PROJECT_NAME}_NDEBUG -D${CMAKE_PROJECT_NAME}_NDebug -D${CMAKE_PROJECT_NAME}_Ndebug -D${CMAKE_PROJECT_NAME}_ndebug)
endif ()
set(CMAKE_LIST_STR "${ANSI_UNDERLINE}CMakeLists.txt${ANSI_RESET}")
set(Vulkan_SDK_STR "${ANSI_UNDERLINE}${ANSI_BLUE}\$Vulkan_SDK${ANSI_RESET}")
find_package(glfw3 REQUIRED NAMES GLFW glfw3)
if (glfw3_FOUND)
	log_ok("${ANSI_BLUE}glfw3${ANSI_RESET} is found: ${ANSI_UNDERLINE}${glfw3_DIR}${ANSI_RESET}")
	add_definitions(-DGLFW_INCLUDE_VULKAN)
endif ()
unset(Vulkan_SDK)
if (DEFINED ENV{VULKAN_SDK})
	set(Vulkan_SDK $ENV{VULKAN_SDK})
elseif (DEFINED ENV{Vulkan_SDK})
	set(Vulkan_SDK $ENV{Vulkan_SDK})
endif ()
if (NOT DEFINED Vulkan_SDK)
	log_warn("Environment ${ANSI_MAGENTA}Vulkan_SDK${ANSI_RESET} is not found, which may result in CMake being unable to locate the Vulkan SDK.")
	if (LINUX)
		log_debug("You may need to add the ${ANSI_BLUE}setup-env.sh${ANSI_RESET} under the Vulkan SDK installation directory to the ${ANSI_MAGENTA}profile${ANSI_RESET}.")
	endif ()
else ()
	log_ok("${ANSI_MAGENTA}Vulkan_SDK${ANSI_RESET} env: ${ANSI_UNDERLINE}${Vulkan_SDK}${ANSI_RESET}")
	if (NOT DEFINED Vulkan_DIR)
		set(Vulkan_DIR ${Vulkan_SDK})
	endif ()
endif ()
find_package(Vulkan REQUIRED MODULE COMPONENTS dxc)
if (Vulkan_FOUND)
	log_ok("${ANSI_BLUE}Vulkan${ANSI_RESET} is found: version ${ANSI_BLUE}${Vulkan_VERSION}${ANSI_RESET}")
	if (DEFINED Vulkan_DIR)
		set(_Include_dirs "include" "Include" "INCLUDE")
		foreach (_Include_dir ${_Include_dirs})
			if (EXISTS "${Vulkan_DIR}/${_Include_dir}/vulkan/vulkan.h" AND EXISTS "${Vulkan_DIR}/${_Include_dir}/vulkan/vulkan.hpp")
				set(Vulkan_Builtin_INCLUDE_SEARCH_DIR "${Vulkan_DIR}/${_Include_dir}")
			endif ()
		endforeach ()
	endif ()
	if (DEFINED Vulkan_Builtin_INCLUDE_SEARCH_DIR AND NOT DEFINED DISABLE_Vulkan_builtin_glm AND EXISTS "${Vulkan_Builtin_INCLUDE_SEARCH_DIR}/glm/glm.hpp")
		set(Vulkan_builtin_glm_FOUND ON)
		set(GLM_INCLUDE_DIRS ${Vulkan_Builtin_INCLUDE_SEARCH_DIR})
		add_library(glm::glm INTERFACE IMPORTED)
		set_target_properties(glm::glm PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIRS})
		add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE -DGLM_FORCE_RADIANS)
		log_ok("${ANSI_BLUE}glm${ANSI_RESET} is found: Vulkan built-in")
		log_debug("You can set ${ANSI_MAGENTA}$DISABLE_Vulkan_builtin_glm${ANSI_RESET} in ${CMAKE_LIST_STR} to avoid using Vulkan built-in glm.")
	endif ()
	if (WIN32)
		if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			log_ok("Win32 Compiler check: Current build toolchain is ${ANSI_BOLD_MAGENTA}MSVC${ANSI_RESET}.")
			set(CMAKE_CXX_COMPILER_IS_MSVC ON)
			add_definitions(-D__PRETTY_FUNCTION__="__func__")
			add_compile_options(
				"/wd4067"   # warning C4067: 预处理器指令后有意外标记 - 应输入换行符
				"/wd4068"   # warning C4068: 未知的杂注“ide”、“clang”
				"/wd4819"   # warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
			)
		else ()
			log_warn("Win32 Compiler check: Current build toolchain is ${ANSI_BOLD_MAGENTA}${CMAKE_CXX_COMPILER_ID}${ANSI_RESET}.")
			if (DISABLE_Vulkan_MSVC)
				log_debug("You can set ${ANSI_MAGENTA}$DISABLE_Vulkan_MSVC${ANSI_RESET} in ${CMAKE_LIST_STR} to re-open compiler check in win32.")
			else ()
				log_debug("You can set ${ANSI_MAGENTA}$DISABLE_Vulkan_MSVC${ANSI_RESET} in ${CMAKE_LIST_STR} to skip compiler check in win32.")
				log_err("Vulkan only works with ${ANSI_BOLD_MAGENTA}MSVC${ANSI_RESET} on Windows, but you are using ${ANSI_BOLD_MAGENTA}${CMAKE_CXX_COMPILER_ID}${ANSI_RESET}.")
			endif ()
		endif ()
	elseif (APPLE)
		if (Vulkan_VERSION GREATER_EQUAL 1.3.216)
			log_debug("In MacOS, Vulkan SDK version >= ${ANSI_BLUE}1.3.216${ANSI_RESET} may cause the ${ANSI_MAGENTA}VK_ERROR_INCOMPATIBLE_DRIVER${ANSI_RESET} issue.")
			set(VULKAN_EXPLICIT_USE_MOLTENVK ON)
			add_definitions(-DVULKAN_EXPLICIT_USE_MOLTENVK -DVulkan_Explicit_Use_MoltenVK)
			log_debug("Due to the issue, ${ANSI_MAGENTA}VULKAN_EXPLICIT_USE_MOLTENVK${ANSI_RESET} is defined.")
			log_newline("You are supposed to check this definitions and configure your Vulkan instance properly in your code.")
			log_newline("Refer to official website for more details: https://vulkan.lunarg.com/doc/sdk/${Vulkan_VERSION}/mac/getting_started.html")
			set(VK_ICD_FILENAMES "${Vulkan_SDK_STR}${ANSI_UNDERLINE}/share/vulkan/icd.d/MoltenVK_icd.json${ANSI_RESET}")
			set(VK_LAYER_PATH "${Vulkan_SDK_STR}${ANSI_UNDERLINE}/share/vulkan/explicit_layer.d${ANSI_RESET}")
		else ()
			set(VK_ICD_FILENAMES "${Vulkan_SDK_STR}${ANSI_UNDERLINE}/etc/vulkan/icd.d/MoltenVK_icd.json${ANSI_RESET}")
			set(VK_LAYER_PATH "${Vulkan_SDK_STR}${ANSI_UNDERLINE}/etc/vulkan/explicit_layer.d${ANSI_RESET}")
		endif ()
		if (NOT DEFINED Vulkan_SDK)
			log_debug("Vulkan environment is not configured so MoltenVK might not find the driver. To avoid this, you may")
			log_newline("1. Run ${ANSI_UNDERLINE}${ANSI_BLUE}setup-env.sh${ANSI_RESET} in where you install Vulkan SDK.")
			log_newline("2. Set environment variables below manually for built binaries:")
			log_newline("   ${ANSI_MAGENTA}VK_ICD_FILENAMES${ANSI_RESET}=${VK_ICD_FILENAMES}")
			log_newline("   ${ANSI_MAGENTA}VK_LAYER_PATH${ANSI_RESET}=${VK_LAYER_PATH}")
			log_newline("   ${ANSI_MAGENTA}VK_ADD_LAYER_PATH${ANSI_RESET}=${VK_LAYER_PATH}")
			log_newline("   ${Vulkan_SDK_STR} refers to the installation path of Vulkan SDK.")
			if (DEFINED Vulkan_DIR)
				log_debug("You set ${ANSI_MAGENTA}$Vulkan_DIR${ANSI_RESET} in ${CMAKE_LIST_STR}, which should be ${Vulkan_SDK_STR} if configured properly.")
			else ()
				log_debug("Most of the time, Vulkan SDK installer default path is similar to ${ANSI_UNDERLINE}$ENV{HOME}/VulkanSDK/${Vulkan_VERSION}.?/macOS${ANSI_RESET}")
			endif ()
		endif ()
	else ()
		log_warn("Vulkan platform is not detected correctly.")
	endif ()
	if (Vulkan_glslc_FOUND)
		log_ok("Vulkan GLSL compiler found: ${ANSI_BLUE}glslc${ANSI_RESET}.")
		set(Vulkan_GLSL_COMPILER ${Vulkan_GLSLC_EXECUTABLE})
		set(Vulkan_GLSL_COMPILER_ID "glslc")
	elseif (Vulkan_glslangValidator_FOUND)
		log_ok("Vulkan GLSL compiler found: ${ANSI_BLUE}glslangValidator${ANSI_RESET}.")
		set(Vulkan_GLSL_COMPILER ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE})
		set(Vulkan_GLSL_COMPILER_ID "glslangValidator")
	else ()
		log_warn("Vulkan GLSL compiler is not detected.")
	endif ()
	if (Vulkan_dxc_exe_FOUND OR Vulkan_dxc_FOUND OR DEFINED Vulkan_dxc_EXECUTABLE)
		log_ok("Vulkan HLSL compiler found: ${ANSI_BLUE}dxc${ANSI_RESET}.")
		log_debug("You can set ${ANSI_MAGENTA}$Vulkan_HLSL_COMPILER_ENTRY${ANSI_RESET} in ${CMAKE_LIST_STR} to override default entry.")
		if (DEFINED Vulkan_HLSL_COMPILER_TARGET_VERSION)
			log_info("${ANSI_BLUE}dxc${ANSI_RESET} will compile shaders to target ${ANSI_BLUE}${Vulkan_HLSL_COMPILER_TARGET_VERSION}${ANSI_RESET}.")
		else ()
			log_debug("You should set ${ANSI_MAGENTA}$Vulkan_HLSL_COMPILER_TARGET_VERSION${ANSI_RESET} in ${CMAKE_LIST_STR} (e.g. ${ANSI_BLUE}6_0${ANSI_RESET}) to set shader target.")
		endif ()
		set(Vulkan_HLSL_COMPILER ${Vulkan_dxc_EXECUTABLE})
		set(Vulkan_HLSL_COMPILER_ID "dxc")
	else ()
		log_warn("Vulkan HLSL compiler is not detected.")
	endif ()
	if (Vulkan_volk_FOUND)
		log_ok("Vulkan API loader found: ${ANSI_BLUE}volk${ANSI_RESET}.")
	else ()
		log_warn("Vulkan API loader is not detected.")
	endif ()
endif ()
macro(target_link_vulkan TARGET)
	if (NOT TARGET ${TARGET})
		log_err("${ANSI_RED}${TARGET}${ANSI_RESET} is not a CMake target!")
	endif ()
	target_include_directories("${TARGET}" PRIVATE ${Vulkan_INCLUDE_DIR})
	target_link_libraries("${TARGET}" glfw ${Vulkan_LIBRARY})
	if (DEFINED Vulkan_DIR AND DEFINED Vulkan_Builtin_INCLUDE_SEARCH_DIR)
		target_include_directories("${TARGET}" PRIVATE ${Vulkan_Builtin_INCLUDE_SEARCH_DIR})
	endif ()
	if (NOT Vulkan_builtin_glm_FOUND)
		target_link_libraries("${TARGET}" glm::glm)
	endif ()
endmacro()
set(GLSL_SHADER_EXT .frag .tesc .tese .vert .geom .comp .glsl)
set(HLSL_SHADER_EXT .ps .vs .ds .gs .cs .hs .hlsl)
set(Vulkan_HLSL_COMPILER_ENTRY "main")
macro(target_add_shaders TARGET)
	set(SHADER_FILES ${ARGN})
	foreach (SHADER_FILE ${SHADER_FILES})
		set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILE}.spv")
		set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}")
		get_filename_component(SHADER_EXT ${SHADER_FILE} EXT)
		get_filename_component(PARENT_DIR "${OUTPUT_FILE}" DIRECTORY)
		string(TOLOWER ${SHADER_EXT} SHADER_EXT)
		if (SHADER_EXT IN_LIST GLSL_SHADER_EXT)
			if (SHADER_EXT STREQUAL ".glsl")
				log_err("Cannot infer shader stage by filename. The extension should be included in following:\n"
					"          .frag, .tesc, .tese, .vert, .geom, .comp")
			elseif (Vulkan_GLSL_COMPILER_ID STREQUAL "glslangValidator")
				set(COMPILE_COMMAND ${Vulkan_GLSL_COMPILER} -V "${INPUT_FILE}" -o "${OUTPUT_FILE}" --quiet)
			elseif (Vulkan_GLSL_COMPILER_ID STREQUAL "glslc")
				set(COMPILE_COMMAND ${Vulkan_GLSL_COMPILER} "${INPUT_FILE}" -o "${OUTPUT_FILE}")
			else ()
				log_err("No supported Vulkan GLSL shaders compiler.")
			endif ()
		elseif ()
			if (SHADER_EXT STREQUAL ".hlsl")
				log_err("Cannot infer shader stage by filename. The extension should be included in following:\n"
					"          .ps, .vs, .ds, .gs, .cs, .hs")
			elseif (Vulkan_HLSL_COMPILER_ID STREQUAL "dxc")
				if (NOT DEFINED Vulkan_HLSL_COMPILER_TARGET_VERSION)
					log_err("Vulkan_HLSL_COMPILER_TARGET_VERSION is not defined, cannot infer shader target profile.")
				endif ()
				string(SUBSTRING ${SHADER_EXT} 1 2 SHADER_STAGE)
				set(SHADER_PROFILE "${SHADER_STAGE}_${Vulkan_HLSL_COMPILER_TARGET_VERSION}")
				set(COMPILE_COMMAND ${Vulkan_HLSL_COMPILER} -E "${Vulkan_HLSL_COMPILER_ENTRY}" -T "${SHADER_PROFILE}" -spirv -Fo "${OUTPUT_FILE}" "${INPUT_FILE}")
			else ()
				log_err("No supported Vulkan HLSL shaders compiler.")
			endif ()
		else ()
			log_err("Unknown Vulkan shaders extension ${ANSI_BOLD}${SHADER_EXT}${ANSI_RESET}.")
		endif ()
		add_custom_command(
			OUTPUT "${OUTPUT_FILE}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PARENT_DIR}"
			COMMAND ${COMPILE_COMMAND}
			DEPENDS "${INPUT_FILE}"
			VERBATIM
		)
		target_sources("${TARGET}" PRIVATE "${INPUT_FILE}" "${OUTPUT_FILE}")
	endforeach ()
endmacro()
if (NOT Vulkan_builtin_glm_FOUND)
	find_package(glm REQUIRED)
	if (glm_FOUND)
		log_ok("${ANSI_BLUE}glm${ANSI_RESET} is found: ${ANSI_UNDERLINE}${glm_DIR}${ANSI_RESET}")
		add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE -DGLM_FORCE_RADIANS)
	endif ()
endif ()
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if (CLANG_FORMAT_EXECUTABLE)
	log_ok("${ANSI_BLUE}clang-format${ANSI_RESET} is found: ${ANSI_UNDERLINE}${CLANG_FORMAT_EXECUTABLE}${ANSI_RESET}")
	macro(target_add_clang_format TARGET)
		if (NOT TARGET ${TARGET})
			log_err("${ANSI_RED}${TARGET}${ANSI_RESET} is not a CMake target!")
		endif ()
		get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
		foreach (SOURCE_FILE ${TARGET_SOURCES})
			if (SOURCE_FILE MATCHES ".*\\.(c|cc|cpp|cxx|h|hh|hpp|hxx)$")
				list(APPEND FORMAT_SOURCES "${SOURCE_FILE}")
			endif ()
		endforeach ()
		if (FORMAT_SOURCES)
			add_custom_target("${TARGET}_clang_format"
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=file ${FORMAT_SOURCES}
				COMMENT "Running clang-format on ${TARGET} sources."
				VERBATIM
			)
			# add_dependencies("${TARGET}" "${TARGET}_clang_format")
			log_info("Clang-format is set for ${ANSI_YELLOW}${ANSI_BOLD}${TARGET}${ANSI_RESET}.")
		else ()
			log_warn("No source files found for ${ANSI_RED}${TARGET}${ANSI_RESET}.")
		endif ()
	endmacro()
else ()
	macro(target_add_clang_format TARGET)
		log_warn("${ANSI_BLUE}clang-format${ANSI_RESET} is not found, cannot setup clang-format to your target.")
	endmacro()
endif ()