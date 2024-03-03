#include "glfw-window.h"
#include "lvk/exception.h"

namespace lvk {

    bool glfw_window::glfw_initialized = false;

    exit_code glfw_window::create_window(std::string_view title, std::string_view icon) {
        if (!glfw_initialized &&
            initialize_glfw() != exit_code::success)
            return exit_code::failure;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (!handle) return exit_code::failure;
        if (!icon.empty()) {
            // todo: 实现设置窗口图标之前，需要先实现图片读取
        }
        glfwSetWindowUserPointer(handle, this);
        glfwSetFramebufferSizeCallback(handle, [](auto window, int w, int h) {
            auto self = reinterpret_cast<glfw_window *>(glfwGetWindowUserPointer(window));
            self->width = w, self->height = h;
        });
        return exit_code::success;
    }

    exit_code glfw_window::initialize_glfw() {
        if (!glfw_initialized) {
            if (!glfwInit()) return exit_code::failure;
            glfw_initialized = true;
        }
        return exit_code::success;
    }

    void glfw_window::terminate_glfw() noexcept {
        if (glfw_initialized) {
            glfwTerminate();
            glfw_initialized = false;
        }
    }

    glfw_window::glfw_window(std::string_view title, int width, int height)
        : width(width), height(height), handle(VK_NULL_HANDLE) {
        if (create_window(title, "") != exit_code::success)
            throw vk_exception("lvk::glfw_window: failed to create window.");
    }

    glfw_window::glfw_window(int width, int height) : glfw_window("", width, height) {}

    glfw_window::~glfw_window() {
        glfwDestroyWindow(handle);
    }

    std::vector<str> glfw_window::get_required_extensions() {
        uint32_t count;
        const char **extensions = glfwGetRequiredInstanceExtensions(&count);
        std::vector<str> result(extensions, extensions + count);
        return result;
    }

} // lvk