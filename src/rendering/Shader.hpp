#pragma once
#include <glad/gl.h>

namespace RR {
	class Shader {
	public:
		GLuint id = -1;

		Shader();
		Shader(GLenum typ, const char* txt);
		~Shader();
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
	};
}
