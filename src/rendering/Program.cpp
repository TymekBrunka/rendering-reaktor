#include "Program.hpp"
#include <cstdlib>
#include <iostream>

namespace RR {
	Program::Program() {
		this->id = glCreateProgram();
	}

	Program::~Program() {
		glDeleteProgram(this->id);
	}

	Program& Program::attachShader(RR::Shader shader) {
		glAttachShader(this->id, shader.id);
        return *this;
	}

	bool Program::link(bool doPrint, bool doExit) {
    	glLinkProgram(this->id);

		int linkStatus;
	    glGetProgramiv(this->id, GL_LINK_STATUS, &linkStatus);
	    if (!linkStatus) {
	        GLsizei message_length = 512;
            std::cout << this->id << "\n";
	        // glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
            std::cout << message_length << "\n";
	        // char* message_buffer = new char[message_length];
            char message_buffer[513] = {0};
	        glGetProgramInfoLog(this->id, 512, NULL, message_buffer);
	        if (doPrint) {
	        	std::cout << "Shader linking error: " << message_buffer << "\n";
	        }
	        glDeleteProgram(this->id);
	        // delete[] message_buffer;
			if (doExit) {
            	exit(EXIT_FAILURE);
			}
            return true;
	    }
	    return false;
	}
}
