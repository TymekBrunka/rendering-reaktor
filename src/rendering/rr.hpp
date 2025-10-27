#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexBuffer.hpp"
#include "FrameBuffer.hpp"
#include "generators.h"

namespace RR {
    void error_callback(int error, const char* description);

    // static const char* description;

    void init();

    GLFWwindow* createWindow(int width, int height, const char* title, int major, int minor);

    GLuint createVertexArray();

    std::string readFile(const char* filepath);

    const GLuint compileShader(GLenum typ, const char* txt);
}
