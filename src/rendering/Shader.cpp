#include "Shader.hpp"
#include <cstdlib>
#include <iostream>

namespace RR {
	Shader::Shader(GLenum typ, const char* txt) {
		const GLuint shader = glCreateShader(typ);
        glShaderSource(shader, 1, &txt, NULL);
        glCompileShader(shader);
        this->id = shader;
	}

    Shader::~Shader() {
        glDeleteShader(this->id);
    }

	bool Shader::errorCheck(bool doPrint, bool doExit) {
		int compilation_status;
        glGetShaderiv(this->id, GL_COMPILE_STATUS, &compilation_status);
        if (compilation_status != GL_TRUE) {
            GLsizei message_length;
            glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
            char* message_buffer = new char[message_length];
            glGetShaderInfoLog(this->id, message_length, NULL, message_buffer);
            if (doPrint) {
                std::cout << "Shader linking error: \x1b[31m" << message_buffer << "\x1b[0m\n";
            };
            glDeleteShader(this->id);
            delete[] message_buffer;
            if (doExit)
            	exit(EXIT_FAILURE);
            return true;
        }
        return false;
	}
}