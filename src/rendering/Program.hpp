#pragma once
#include <glad/gl.h>
#include "Errors.hpp"
#include "Shader.hpp"

namespace RR {
	class Program {
	public:
		GLuint id;

		Program();
		~Program();
		Program& attachShader(RR::Shader& shader);
		RR::result link(bool doPrint, bool doExit);
	};
}
