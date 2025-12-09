#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>

// #include "glm/ext.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

Camera::Camera(glm::vec3 position, float orientation_x, float orientation_y) {
	this->position = position;
	this->orientation_x = orientation_x;
	this->orientation_y = orientation_y;

	this->update_view(position, orientation_x, orientation_y);
}

void Camera::update_projection(int width, int height, float fov) {
	this->priv.projection = glm::perspective(fov, (float)width / (float)height, 0.01f, 1000.0f);
}

void Camera::update_view(glm::vec3 position, float orientation_x, float orientation_y) {
	this->position = position;
	this->orientation_x = orientation_x;
	this->orientation_y = orientation_y;

	// float sinx = glm::sin(orientation[0]);
	// float cosx = glm::sin(orientation[0]);
	// float siny = glm::sin(orientation[1]);
	float cosy = glm::cos(orientation_x);

	glm::vec3 forward(
		glm::sin(orientation_x) * cosy,
		glm::sin(orientation_y),
		glm::cos(orientation_x) * cosy
	);

	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(right, forward);

	// std::cout << glm::sin(orientation_x) << "\n";
	std::cout << "forward: " << glm::to_string(forward) << "\n";

	this->priv.view = glm::lookAt(position, position + forward, up);
	this->priv.view_skybox = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), forward, up);
	std::cout << "view: " << glm::to_string(this->priv.view) << "\n";
}

void Camera::move_and_set_orientation(float input[], float deltatime, float orientation_x, float orientation_y) {
	this->orientation_x = orientation_x;
	this->orientation_y = orientation_y;

	float sinx = glm::sin(orientation_x);
	float cosx = glm::sin(orientation_x);
	// float siny = glm::sin(orientation[1]);
	float cosy = glm::sin(orientation_x);

	glm::vec3 forward(
		sinx * cosy,
		glm::sin(orientation_y),
		cosx * cosy
	);

	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(right, forward);

	glm::vec3 movement_right(-sinx, 0.0, cosx);
	this->position = this->position + 40.f * deltatime * glm::vec3(forward * input[1] + right * input[0]);

	this->priv.view = glm::lookAt(this->position, this->position + forward, up);
	this->priv.view_skybox = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), forward, up);
}

void Camera::computeMatricies() {
	this->priv.camera = this->priv.projection * this->priv.view ;
	this->priv.camera_skybox = this->priv.projection * this->priv.view_skybox;
	glm::vec4 unit(1.0f, 1.0f, 1.0f, 1.0f);
	std::cout << glm::to_string(unit * this->priv.camera) << "\n";
	std::cout << "perspective: " << glm::to_string(this->priv.projection) << "\n";
	std::cout << "camera: " << glm::to_string(this->priv.camera) << "\n";
}

const CameraPrivates& Camera::read() {
	return this->priv;
}
