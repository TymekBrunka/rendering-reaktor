#pragma once
#include <glad/glad.h>
#include <limits>
#include "Shader.hpp"

namespace RR {
	class Program {
	public:
    static constexpr GLuint INVALID_ID = std::numeric_limits<GLuint>::max();
		GLuint id = INVALID_ID;

		Program();
		Program(const char* x);
		~Program();
		Program(Program&& other) noexcept;
		Program& operator=(Program&& other) noexcept;

		Program& attachShader(RR::Shader& shader);
		Program& link();
	};
}
