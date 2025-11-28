#include "Shader.hpp"
#include <cstdlib>
#include <iostream>

namespace RR {
	Shader::Shader(GLenum typ, const char* txt) {
		const GLuint shader = glCreateShader(typ);
        glShaderSource(shader, 1, &txt, NULL);
        glCompileShader(shader);
        this->id = shader;

		int compilation_status;
        glGetShaderiv(this->id, GL_COMPILE_STATUS, &compilation_status);
        if (!compilation_status) {
            // GLsizei message_length;
            // glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
            // char* message_buffer = new char[message_length];
            char message_buffer[512] = {0};
            glGetShaderInfoLog(this->id, 512, NULL, message_buffer);
            char message[580] = {0};
            std::snprintf(message, 256, "Shader linking error: %s", message_buffer);
            glDeleteShader(this->id);

            throw std::string(message);
            // delete[] message_buffer;
        }
	}

    Shader::~Shader() {
        if (this->id != -1) glDeleteShader(this->id);
    }

    Shader::Shader(Shader&& other) noexcept {
		this->id = other.id;
		other.id = -1;
    }
    Shader& Shader::operator=(Shader&& other) noexcept {
		if (this != &other) {
			this->id = other.id;
			other.id = -1;
		}
		return *this;
    }
}
