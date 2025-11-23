#pragma once
#include <glad/gl.h>
#include "Shader.hpp"

namespace RR {
	class Program {
	public:
		GLuint id = -1;

		Program();
		~Program();
		Program(Program&& other) noexcept;
		Program& operator=(Program&& other) noexcept;

		Program& attachShader(RR::Shader& shader);
		Program& link();
	};
}
