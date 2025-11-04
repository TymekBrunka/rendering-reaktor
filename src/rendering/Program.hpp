#pragma once
#include <glad/gl.h>
#include "Shader.hpp"

namespace RR {
	class Program {
	public:
		GLuint id;

		Program();
		~Program();
		Program& attachShader(RR::Shader shader);
		bool link(bool doPrint, bool doExit);
	};
}
