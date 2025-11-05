#pragma once
#include <glad/gl.h>
#include <string>

namespace RR {
	struct result {
		GLint id;
		std::string message;
	};

	struct enum_result {
		GLenum id;
		std::string message;
	};
}
