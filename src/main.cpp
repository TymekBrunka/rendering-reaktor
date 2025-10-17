// #define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
// #include <GLFW/glfw3.h>
#include <nfd.h>
#include "rr.hpp"

#include <iostream>
#include "triangleVB.cpp"

int main() {
	RR::init();
	GLFWwindow* window = RR::createWindow(640, 480, "OpenGL Triangle", 3, 2); // #version 320
	if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window); // context must be set first
	gladLoadGL(glfwGetProcAddress); // only then we can load
	glfwSwapInterval(1);

	std::string vertex_text = RR::readFile("shaders/empty.glsl");
	std::string fragment_text = RR::readFile("shaders/empty.glsl");

	GLuint vertex_shader = RR::compileShader(GL_VERTEX_SHADER, vertex_text.c_str());
	GLuint fragment_shader = RR::compileShader(GL_FRAGMENT_SHADER, fragment_text.c_str());

	const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    triangle_vertex verticies[] = { 
    	{glm::vec2(0, 1), glm::vec3(1.0, 0.0, 0.0)},
    	{glm::vec2(-1, -1), glm::vec3(0.0, 1.0, 0.0)},
    	{glm::vec2(1, -1), glm::vec3(0.0, 0.0, 1.0)},
    };

    TriangleVertexBuffer vb = TriangleVertexBuffer(program, (void*)verticies, 3, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        // glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
