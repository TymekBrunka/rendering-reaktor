#include "rr.hpp"

#include <stdio.h>
#include <cstdlib>
#include <glm/vec2.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

struct a {
    glm::vec2 b;
};

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
        std::cout << txt << "\n\n";
        glShaderSource(shader, 1, &txt, NULL);
        glCompileShader(shader);

        return shader;
    }
}
