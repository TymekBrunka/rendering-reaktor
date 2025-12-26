#include <cstdint>
#include <cstdlib>
#include <glad/gl.h>
#include <nfd.h>
#include "glm/fwd.hpp"
#include "rendering/imgui/imgui.h"
#include "rr.hpp"
#include "Camera.hpp"

#include "imgui.h"
#include "imgui_boilerplate.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "stb_image.h"
#include <iostream>

Camera camera(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec2(0.0f, 0.0f));
int Gwidth;
int Gheight;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	camera.holding_rmb = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	camera.holding_rmb = false;
    }
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
		glm::vec2 temp((float)x, (float)y);
        if (camera.holding_rmb) {
        	// std::cout << x << ", " << y << "hai\n";
            camera.orientation -= glm::radians(temp - camera.last_mouse_pos) / 2.0f;
            camera.orientation = glm::vec2(glm::mod(camera.orientation.x, (2.0f * 3.14f)), glm::clamp(camera.orientation.y, (-0.5f * 3.14f) + 0.0001f, (0.5f * 3.14f) + 0.0001f));
            camera.update_view(glm::vec3(0.0f), camera.orientation);
            camera.computeMatricies();
        }
        camera.last_mouse_pos[0] = x;
        camera.last_mouse_pos[1] = y;
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Gwidth = width;
	Gheight = height;
	glfwGetFramebufferSize(window, &width, &height);
    // const float ratio = width / (float) height;
    glViewport(0, 0, width, height);
	camera.update_projection(width, height, 120);
	camera.computeMatricies();
}

struct skybox_vert {
	RR::vec3 pos;
	RR::vec2 uv;
};

GLint location_pos;
GLint location_uv;

int main() {
	RR::init();
	// imgui_boilerplate();
	GLFWwindow* window = RR::createWindow(640, 480, "Reaktory", 3, 2); // #version 320
	if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window); // context must be set first
	gladLoadGL(glfwGetProcAddress); // only then we can load
	glfwSwapInterval(1);

	RR::image_data icon_data = RR::readImage("src/icon.png", 4);
	{
		GLFWimage icon[1];
		icon[0].width = icon_data.width;
		icon[0].height = icon_data.height;
		icon[0].pixels = icon_data.data;
		glfwSetWindowIcon(window, 1, icon);
	}

	RR::Texture2d icon(icon_data);
	stbi_image_free(icon_data.data);

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
    RR::image_data img = RR::readImage("src/cubemap.png", 4);
    RR::Texture2d texture(img);
    stbi_image_free(img.data);

    img = RR::readImage("src/icons.png", 4);
    RR::Texture2d icons(img);
    stbi_image_free(img.data);

	glUseProgram(program.id);
    texture.bindToSlotAndName(program, 0, "skybox");

    icon.bindToSlot(4);
    icons.bindToSlot(3);

	skybox_vert skybox_verticies[] = {
		//back
		{{-1.0,  1.0, -1.0}, {0.75, 0.665}},
		{{-1.0, -1.0, -1.0}, {0.75, 0.334}},
		{{ 1.0, -1.0, -1.0}, {1.00, 0.334}},
		{{ 1.0,  1.0, -1.0}, {1.00, 0.665}},

		//front
		{{ 1.0,  1.0,  1.0}, {0.25, 0.665}},
		{{ 1.0, -1.0,  1.0}, {0.25, 0.334}},
		{{-1.0, -1.0,  1.0}, {0.50, 0.334}},
		{{-1.0,  1.0,  1.0}, {0.50, 0.665}},

		//right
		{{ 1.0,  1.0, -1.0}, {0.00, 0.665}},
		{{ 1.0, -1.0, -1.0}, {0.00, 0.334}},
		{{ 1.0, -1.0,  1.0}, {0.25, 0.334}},
		{{ 1.0,  1.0,  1.0}, {0.25, 0.665}},

		//left
		{{-1.0,  1.0,  1.0}, {0.50, 0.665}},
		{{-1.0, -1.0,  1.0}, {0.50, 0.334}},
		{{-1.0, -1.0, -1.0}, {0.75, 0.334}},
		{{-1.0,  1.0, -1.0}, {0.75, 0.665}},

		//bottom
		{{-1.0,  -1.0, -1.0}, {0.499, 0.000}},
		{{-1.0,  -1.0,  1.0}, {0.499, 0.332}},
		{{ 1.0,  -1.0,  1.0}, {0.251, 0.332}},
		{{ 1.0,  -1.0, -1.0}, {0.251, 0.000}},

		//top
		{{-1.0,  1.0,  1.0}, {0.499, 0.667}},
		{{-1.0,  1.0, -1.0}, {0.499, 1.000}},
		{{ 1.0,  1.0, -1.0}, {0.251, 1.000}},
		{{ 1.0,  1.0,  1.0}, {0.251, 0.667}},
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

	iminit(window, true);

	RR::VertexBuffer<skybox_vert> skybox_vb(skybox_verticies, sizeof(skybox_verticies) / sizeof(skybox_vert), GL_STATIC_DRAW);
	RR::IndexBuffer skybox_ib(skybox_indecies, sizeof(skybox_indecies) / sizeof(GLuint), GL_STATIC_DRAW);

	RR_AUTOATTRIB(skybox_vert, pos, GL_TRUE);
	RR_AUTOATTRIB(skybox_vert, uv, GL_TRUE);

	//Camera
	camera.update_projection(800, 600, 120);
	camera.computeMatricies();

	const GLint rotatm4 = glGetUniformLocation(program.id, "rotat");

	float currentFrame, lastFrame, deltaTime;
	skybox_vb.bind();
	skybox_ib.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glfwGetFramebufferSize(window, &Gwidth, &Gheight);
    // const float ratio = width / (float) height;
    glViewport(0, 0, Gwidth, Gheight);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imNewFrame();
        setupDocking(Gwidth, Gheight);

        glUseProgram(program.id);
        // texture.bindToSlot(0);
        glBindVertexArray(skybox_va);

        glm::mat4 mat = glm::mat4(1.0f);
        glm::mat4 rotat = glm::rotate(mat, glm::radians(currentFrame) * 20, glm::vec3(0.0f, 1.0f, 0.0f));
        // rotat = glm::rotate(rotat, glm::radians(currentFrame) * 20, glm::vec3(1.0f, 0.0f, 0.0f));
		mat = camera.read().camera_skybox;
        glUniformMatrix4fv(rotatm4, 1, GL_FALSE, (const GLfloat*) glm::value_ptr(mat));

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, sizeof(skybox_indecies) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // ImGui::SetNextWindowPos(ImVec2(0, 0));
        // ImGui::SetNextWindowSize(ImVec2(200, Gheight));

        ImGui::Begin("Hello", NULL);
        ImGui::Image((ImTextureID)icon.id, ImVec2(25, 25));
        ImGui::SetCursorPos(ImVec2(40, 32));
        ImGui::Text("Reaktory");
        // ImGui::SetCursorPos(ImVec2(0, 60));
        ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 1.0), ImVec2(0.25, 0.75));
        ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 0.5), ImVec2(0.25, 0.25));
        ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 0.5), ImVec2(0.25, 0.25));
        ImGui::End();

        // ImGui::End();
        imrender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
