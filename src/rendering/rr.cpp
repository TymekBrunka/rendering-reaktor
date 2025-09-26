#define GLFW_INCLUDE_NONE
#define GLAD_GL_IMPLEMENTATION
#include "rr.hpp"

#include <stdio.h>
#include <cstdlib>

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
            exit(EXIT_FAILURE);
        }

        gladLoadGL(glfwGetProcAddress);
    }
}
