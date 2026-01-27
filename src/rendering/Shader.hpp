#pragma once
#include <glad/glad.h>
#include <limits>

namespace RR {
	class Shader {
	public:
    static constexpr GLuint INVALID_ID = std::numeric_limits<GLuint>::max();
		GLuint id = INVALID_ID;

		Shader();
		Shader(GLenum typ, const char* txt);
		~Shader();
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
	};
}
