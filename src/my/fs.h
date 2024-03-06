#ifndef MY_FS_H
#define MY_FS_H
#pragma once

#include "fwd.h"

#include <stdexcept>

namespace my {

	class fs_exception final : public std::runtime_error {
	public:
		explicit fs_exception(const std::string &what);
	};

	buffer read_file_sync(const char *path);

}

#endif //MY_FS_H
