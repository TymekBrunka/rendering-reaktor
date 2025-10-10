#include "generators.h"
#define GLAD_GL_IMPLEMENTATION
#include "rr.hpp"

#include <stdio.h>
#include <cstdlib>
#include <glm/vec2.hpp>

#include 

struct a {
    glm::vec2 b;
}

namespace RR {
    void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: \x1b[31m%s\x1b[0m\n", description);
    }

    // static const char* description;

    void init() {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
        {
        //     glfwGetError(&description);
        //     printf("Error: %s\n", description);
            exit(-1);
        }

        gladLoadGL(glfwGetProcAddress);

        const GLint gex = glGetUniformLocation(program, "gex");
        RR_AUTOATTRIB(a, b, gex);

    }

    GLFWwindow* createWindow(int width, int height, const char* title, int major, int minor) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
        return window;
    }
}