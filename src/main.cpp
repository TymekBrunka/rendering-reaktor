// #define GLAD_GL_IMPLEMENTATION
#include <cstdlib>
#include <glad/gl.h>
// #include <GLFW/glfw3.h>
#include <nfd.h>
#include "rendering/Shader.hpp"
#include "rendering/rr.hpp"
#include "rr.hpp"

#include <iostream>
#include "triangleVB.cpp"

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

	RR::Shader vertex_shader = RR::Shader(GL_VERTEX_SHADER, vertex_text.c_str());
	RR::Shader fragment_shader = RR::Shader(GL_FRAGMENT_SHADER, fragment_text.c_str());

    vertex_shader.errorCheck(true, true);
    fragment_shader.errorCheck(true, true);

	const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader.id);
    glAttachShader(program, fragment_shader.id);
    glLinkProgram(program);


    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLsizei message_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &message_length);
        char* message_buffer = new char[message_length];
        glGetProgramInfoLog(program, message_length, NULL, message_buffer);
        std::cout << "Shader linking error: " << message_buffer << "\n";
        glDeleteProgram(program);
        delete[] message_buffer;
    }


    triangle_vertex verticies[] = { 
        { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
        { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
        { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
    };

    RR::VertexBuffer<triangle_vertex> vb = RR::VertexBuffer(program, verticies, 3, GL_STATIC_DRAW);
    GLuint vertex_array = RR::createVertexArray();
    glBindVertexArray(vertex_array);
    setup_triangle_vertex_array_attribs(program);
    // RR::FrameBuffer fb(300,100);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    // exit(EXIT_SUCCESS);
}
