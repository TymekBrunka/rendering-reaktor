#pragma once
#include <glad/gl.h>
#include "Errors.hpp"

namespace RR {
	class Shader {
	public:
		GLuint id;

		Shader(GLenum typ, const char* txt);
		~Shader();

		RR::result errorCheck(bool doPrint, bool doExit);
	};
}