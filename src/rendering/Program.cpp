#include "Program.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

namespace RR {
	Program::Program() {
		this->id = glCreateProgram();
	}

	Program::~Program() {
		glDeleteProgram(this->id);
	}

	Program& Program::attachShader(RR::Shader& shader) {
		glAttachShader(this->id, shader.id);
        return *this;
	}

	RR::result Program::link(bool doPrint, bool doExit) {
    	glLinkProgram(this->id);

    	RR::result ret = { 0, std::string() };

		int linkStatus;
	    glGetProgramiv(this->id, GL_LINK_STATUS, &linkStatus);
	    if (!linkStatus) {
	    	ret.id = linkStatus;
	        // GLsizei message_length;
	        // glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);
	        // char* message_buffer = new char[message_length];
            char message_buffer[512] = {0};
	        glGetProgramInfoLog(this->id, 512, NULL, message_buffer);
	        if (doPrint) {
	        	std::cout << "Shader linking error: " << message_buffer << "\n";
	        } else {
	        	ret.message = std::string(message_buffer);
	        }
	        glDeleteProgram(this->id);
	        // delete[] message_buffer;
			if (doExit) {
            	exit(EXIT_FAILURE);
			}
            return ret;
	    }
	    return ret;
	}
}
