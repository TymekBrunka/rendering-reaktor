#include <cstdlib>
#include <glad/gl.h>
#include <nfd.h>
#include "VertexBuffer.hpp"
#include "generators.h"
#include "rr.hpp"

#include "stb_image.h"
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct skybox_vert {
	RR::vec3 pos;
	RR::vec2 uv;
};

GLint location_pos;
GLint location_uv;

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

	RR::Program program;
	auto [vertex_shader, fragment_shader] = [&program]() {
		std::string vertex_text = RR::readFile("src/shaders/skybox.vertex.glsl");
		std::string fragment_text = RR::readFile("src/shaders/skybox.frag.glsl");

		try {
			RR::Shader vertex_shader = RR::Shader(GL_VERTEX_SHADER, vertex_text.c_str());
			RR::Shader fragment_shader = RR::Shader(GL_FRAGMENT_SHADER, fragment_text.c_str());
			program
				.attachShader(vertex_shader)
				.attachShader(fragment_shader);
			program.link();

			return std::make_tuple(
					std::move(vertex_shader),
					std::move(fragment_shader));
	} catch(std::string ex) {
		std::cout << ex << "\n";
		exit(1);
		}
	}();

	// RR::FrameBuffer fb(600, 800, 1);

    stbi_set_flip_vertically_on_load(true);
    RR::image_data img = RR::readImage("src/cubemap.png");
    RR::Texture2d texture(img);
    stbi_image_free(img.data);

	glUseProgram(program.id);
    texture.bindToSlotAndName(program, 0, "skybox");

	skybox_vert skybox_verticies[] = {
		{{-1.0,  1.0, -1.0}, {0.75, 0.333}},
		{{-1.0, -1.0, -1.0}, {0.75, 0.666}},
		{{ 1.0, -1.0, -1.0}, {1.00, 0.666}},
	};
	RR::VertexBuffer<skybox_vert> skybox_vb(skybox_verticies, sizeof(skybox_verticies) / sizeof(skybox_vert), GL_STATIC_DRAW);
	GLuint skybox_va = RR::createVertexArray();

	glBindVertexArray(skybox_va);
	RR_AUTOATTRIB(skybox_vert, pos, GL_TRUE);
	RR_AUTOATTRIB(skybox_vert, uv, GL_TRUE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program.id);
        // texture.bindToSlot(0);
        glBindVertexArray(skybox_va);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
