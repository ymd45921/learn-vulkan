#include "fs.h"
#include "buffer.h"

#include <iostream>
#include <fstream>

namespace my {

	fs_exception::fs_exception(const std::string &what)
		: std::runtime_error(what) {}

	buffer read_file_sync(const char *path) {
		std::ifstream file(path, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw fs_exception("my::read_file_sync: failed to open file!");
		auto size = file.tellg();
		buffer buffer(size);
		file.seekg(0).read(buffer.as<char>(), size), file.close();
		return buffer;
	}

}
