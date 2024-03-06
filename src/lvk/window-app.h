#ifndef LVK_WINDOW_APP_H
#define LVK_WINDOW_APP_H
#pragma once

#include "app.h"
#include "instance.h"
#include "lvk.h"
#include "lvk/objects/glfw-window.h"

namespace lvk {

	class window_app : public app {

		glfw_window wnd;
		instance vki;

	public:
		window_app(std::string_view title, int width = 800, int height = 600);

		window_app(const window_app &) = delete;
	};

} // namespace lvk


#endif // LVK_WINDOW_APP_H
