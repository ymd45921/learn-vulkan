#include "window-app.h"
#include "instance.h"

lvk::window_app::window_app(std::string_view title, int width, int height)
        : wnd(title, width, height), vki(title, glfw_window::get_required_extensions()) {}
