#include <cstdlib>
#include <glad/gl.h>
#include <nfd.h>
#include "rr.hpp"

#include "stb_image.h"
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	RR::init();
	GLFWwindow* window = RR::createWindow(640, 480, "OpenGL Triangle", 3, 2); // #version 320
	if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window); // context must be set first
	gladLoadGL(glfwGetProcAddress); // only then we can load
	glfwSwapInterval(1);

	std::string vertex_text = RR::readFile("src/shaders/triangle.vertex.glsl");
	std::string fragment_text = RR::readFile("src/shaders/triangle.frag.glsl");

	try {
		RR::Shader vertex_shader = RR::Shader(GL_VERTEX_SHADER, vertex_text.c_str());
		RR::Shader fragment_shader = RR::Shader(GL_FRAGMENT_SHADER, fragment_text.c_str());
	} catch(std::string ex) {
		std::cout << ex << "\n";
		exit(1);
	}
}