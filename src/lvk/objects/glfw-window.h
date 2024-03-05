#ifndef LVK_GLFW_WINDOW_H
#define LVK_GLFW_WINDOW_H

#include "GLFW/glfw3.h"
#include "lvk/fwd.h"

#include <string_view>
#include <vector>

namespace lvk {

	class glfw_window {

		constexpr static int default_width = 800, default_height = 600;
		static bool glfw_initialized;

		GLFWwindow *handle;
		int width, height;

		exit_code create_window(std::string_view title, std::string_view w = "");

	public:
		explicit glfw_window(std::string_view title, int width = default_width,
							 int height = default_height);

		glfw_window(int width, int height);

		~glfw_window();

		using self_t = glfw_window;

		GLFWwindow *operator*() const { return handle; }

		static std::vector<str> get_required_extensions();

		static exit_code initialize_glfw();

		// todo: 如何在合适的时候反初始化 glfw？
		static void terminate_glfw() noexcept;
	};

} // namespace lvk

#endif // LVK_GLFW_WINDOW_H
