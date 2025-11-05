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

	RR::result Shader::errorCheck(bool doPrint, bool doExit) {
        RR::result ret = { 0, std::string() };

		int compilation_status;
        glGetShaderiv(this->id, GL_COMPILE_STATUS, &compilation_status);
        if (!compilation_status) {
            ret.id = compilation_status;
            // GLsizei message_length;
            // glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
            // char* message_buffer = new char[message_length];
            char message_buffer[512] = {0};
            glGetShaderInfoLog(this->id, 512, NULL, message_buffer);
            if (doPrint) {
                std::cout << "Shader linking error: \x1b[31m" << message_buffer << "\x1b[0m\n";
            } else {
                ret.message = std::string(message_buffer);
            }
            glDeleteShader(this->id);
            // delete[] message_buffer;
            if (doExit)
            	exit(EXIT_FAILURE);
            return ret;
        }
        return ret;
	}
}
