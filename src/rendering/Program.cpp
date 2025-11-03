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

	&Program Program::attachShader(RR::Shader shader) {
		glAttachShader(this->id, shader->id);
	}

	bool Program::link(bool doPrint, bool doExit) {
    	glLinkProgram(program);

		int linkStatus;
	    glGetProgramiv(this->id, GL_LINK_STATUS, &linkStatus);
	    if (linkStatus != GL_TRUE) {
	        GLsizei message_length;
	        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
	        char* message_buffer = new char[message_length];
	        glGetProgramInfoLog(this->id, message_length, NULL, message_buffer);
	        if (doPrint) {
	        	std::cout << "Shader linking error: " << message_buffer << "\n";
	        }
	        glDeleteProgram(this->id);
	        delete[] message_buffer;
			if (doExit) {
            	exit(EXIT_FAILURE);
			}
            return true;
	    }
	    return false;
	}
}