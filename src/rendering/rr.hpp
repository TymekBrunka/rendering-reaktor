#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace RR {
    extern void error_callback(int error, const char* description);

    // static const char* description;

    extern void init();

    GLFWwindow* createWindow(int width, int height, const char* title, int major, int minor);
}
