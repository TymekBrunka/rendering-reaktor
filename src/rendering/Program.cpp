#include "Program.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

namespace RR {
	Program::Program() {
		this->id = glCreateProgram();
	}

	Program::~Program() {
        if (this->id != -1) glDeleteProgram(this->id);
	}

	Program& Program::attachShader(RR::Shader& shader) {
		glAttachShader(this->id, shader.id);
        return *this;
	}

	Program& Program::link() {
    	glLinkProgram(this->id);

		int linkStatus;
	    glGetProgramiv(this->id, GL_LINK_STATUS, &linkStatus);
	    if (!linkStatus) {
	        // GLsizei message_length;
	        // glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
	        // char* message_buffer = new char[message_length];
            char message_buffer[512] = {0};
	        glGetProgramInfoLog(this->id, 512, NULL, message_buffer);
			throw ("Shader linking error: " + std::string(message_buffer));
	        glDeleteProgram(this->id);
	        // delete[] message_buffer;
			return *this;
	    }
		return *this;
	}

	Program::Program(Program&& other) noexcept {
		this->id = other.id;
		other.id = -1;
	}

	Program& Program::operator=(Program&& other) noexcept {
		if (this != &other) {
			this->id = other.id;
			other.id = -1;
		}
		return *this;
	}
}
