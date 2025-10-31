#pragma once
#include <glad/gl.h>

namespace RR {
	class Shader {
	public:
		GLuint id;

		Shader(GLenum typ, const char* txt);

		bool errorCheck(bool doPrint, bool doExit);
	};
}