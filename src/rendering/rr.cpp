#include "rr.hpp"

#include <stdio.h>
#include <cstdlib>
#include <glm/vec2.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

namespace RR {
    void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: \x1b[31m%s\x1b[0m\n", description);
    }

    // static const char* description;

    void init() {
        glfwSetErrorCallback(RR::error_callback);

        if (!glfwInit())
        {
        //     glfwGetError(&description);
        //     printf("Error: %s\n", description);
            exit(-1);
        }
    }

    GLFWwindow* createWindow(int width, int height, const char* title, int major, int minor) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
        return window;
    }

    GLuint createVertexArray() {
        GLuint va;
        glGenVertexArrays(1, &va);
        glBindVertexArray(va);
        return va;
    }

    std::string readFile(const char* filepath){
        std::ifstream inFile;
        inFile.open(filepath); //open the input file

        std::stringstream strStream;
        strStream << inFile.rdbuf(); //read the file
        std::string str = strStream.str(); //str holds the content of the file
        return str;
    }

    const GLuint compileShader(GLenum typ, const char* txt) {
        const GLuint shader = glCreateShader(typ);
        // std::cout << txt << "\n\n";
        glShaderSource(shader, 1, &txt, NULL);
        glCompileShader(shader);

        int compilation_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
        if (compilation_status != GL_TRUE) {
            GLsizei message_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &message_length);
            char* message_buffer = new char[message_length];
            glGetShaderInfoLog(shader, message_length, NULL, message_buffer);
            std::cout << "Shader linking error: \x1b[31m" << message_buffer << "\x1b[0m\n";
            glDeleteShader(shader);
            delete[] message_buffer;
            exit(EXIT_FAILURE);
        }

        return shader;
    }
}
