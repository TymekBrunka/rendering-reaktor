// #define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
// #include <GLFW/glfw3.h>
#include <nfd.h>
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

	GLuint vertex_shader = RR::compileShader(GL_VERTEX_SHADER, vertex_text.c_str());
	GLuint fragment_shader = RR::compileShader(GL_FRAGMENT_SHADER, fragment_text.c_str());

    // int compilation_status;
    // glGetShaderiv(vertex_shader, GL_LINK_STATUS, &compilation_status);
    // if (compilation_status != GL_TRUE) {
    //     GLsizei message_length;
    //     glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &message_length);
    //     char* message_buffer = new char[message_length];
    //     glGetShaderInfoLog(vertex_shader, message_length, NULL, message_buffer);
    //     std::cout << "Shader linking error: " << message_buffer << "\n";
    //     glDeleteShader(vertex_shader);
    //     delete[] message_buffer;
    // }

    // glGetShaderiv(fragment_shader, GL_LINK_STATUS, &compilation_status);
    // if (compilation_status != GL_TRUE) {
    //     GLsizei message_length;
    //     glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &message_length);
    //     char* message_buffer = new char[message_length];
    //     glGetShaderInfoLog(fragment_shader, message_length, NULL, message_buffer);
    //     std::cout << "Shader linking error: " << message_buffer << "\n";
    //     glDeleteShader(fragment_shader);
    //     delete[] message_buffer;
    // }

	const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
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

    TriangleVertexBuffer vb = TriangleVertexBuffer(program, (void*)verticies, 3, GL_STATIC_DRAW);
    vb.setup_attributes(program);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        // glBindVertexArray(vertex_array);
        vb.Apply();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
