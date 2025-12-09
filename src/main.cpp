#include <cstdlib>
#include <glad/gl.h>
#include <nfd.h>
#include "glm/fwd.hpp"
#include "rr.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
    RR::image_data img = RR::readImage("src/cubemap_dbg.png");
    RR::Texture2d texture(img);
    stbi_image_free(img.data);

	glUseProgram(program.id);
    texture.bindToSlotAndName(program, 0, "skybox");

	skybox_vert skybox_verticies[] = {
		//back
		{{-1.0,  1.0, -1.0}, {0.75, 0.666}},
		{{-1.0, -1.0, -1.0}, {0.75, 0.333}},
		{{ 1.0, -1.0, -1.0}, {1.00, 0.333}},
		{{ 1.0,  1.0, -1.0}, {1.00, 0.666}},

		//front
		{{ 1.0,  1.0,  1.0}, {0.25, 0.666}},
		{{ 1.0, -1.0,  1.0}, {0.25, 0.333}},
		{{-1.0, -1.0,  1.0}, {0.50, 0.333}},
		{{-1.0,  1.0,  1.0}, {0.50, 0.666}},

		//right
		{{ 1.0,  1.0, -1.0}, {0.50, 0.666}},
		{{ 1.0, -1.0, -1.0}, {0.50, 0.333}},
		{{ 1.0, -1.0,  1.0}, {0.75, 0.333}},
		{{ 1.0,  1.0,  1.0}, {0.75, 0.666}},

		//left
		{{-1.0,  1.0,  1.0}, {0.00, 0.666}},
		{{-1.0, -1.0,  1.0}, {0.00, 0.333}},
		{{-1.0, -1.0, -1.0}, {0.25, 0.333}},
		{{-1.0,  1.0, -1.0}, {0.25, 0.666}},

		//bottom
		{{-1.0,  -1.0, 1.0}, {0.25, 0.333}},
		{{-1.0,  -1.0, -1.0}, {0.25, 0.000}},
		{{ 1.0,  -1.0, -1.0}, {0.50, 0.000}},
		{{ 1.0,  -1.0, 1.0}, {0.50, 0.333}},

		//top
		{{-1.0,  1.0, -1.0}, {0.25, 0.666}},
		{{-1.0,  1.0, 1.0}, {0.25, 1.000}},
		{{ 1.0,  1.0, 1.0}, {0.50, 1.000}},
		{{ 1.0,  1.0, -1.0}, {0.50, 0.666}},
	};

	GLuint skybox_indecies[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	GLuint skybox_va = RR::createVertexArray();
	glBindVertexArray(skybox_va);
	RR::VertexBuffer<skybox_vert> skybox_vb(skybox_verticies, sizeof(skybox_verticies) / sizeof(skybox_vert), GL_STATIC_DRAW);
	RR::IndexBuffer skybox_ib(skybox_indecies, sizeof(skybox_indecies) / sizeof(GLuint), GL_STATIC_DRAW);

	RR_AUTOATTRIB(skybox_vert, pos, GL_TRUE);
	RR_AUTOATTRIB(skybox_vert, uv, GL_TRUE);

	//Camera
	Camera cam(glm::vec3(0.1f, 0.1f, 0.1f), 0, 0);
	cam.update_projection(800, 600, 360);
	cam.computeMatricies();

	const GLint rotatm4 = glGetUniformLocation(program.id, "rotat");

	float currentFrame, lastFrame, deltaTime;
	skybox_vb.bind();
	skybox_ib.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;
        glViewport(0, 0, width, height);

        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.id);
        // texture.bindToSlot(0);
        glBindVertexArray(skybox_va);

        glm::mat4 mat = glm::mat4(1.0f);
        glm::mat4 rotat = glm::rotate(mat, glm::radians(currentFrame) * 20, glm::vec3(0.0f, 1.0f, 0.0f));
        rotat = glm::rotate(rotat, glm::radians(currentFrame) * 20, glm::vec3(1.0f, 0.0f, 0.0f));
		mat = cam.read().camera_skybox * rotat;
        glUniformMatrix4fv(rotatm4, 1, GL_FALSE, (const GLfloat*) glm::value_ptr(mat));

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, sizeof(skybox_indecies) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
