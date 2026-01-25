#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>

// // #include "glm/ext.hpp"
// #define GLM_ENABLE_EXPERIMENTAL
// #include "glm/gtx/string_cast.hpp"

Camera::Camera(glm::vec3 position, glm::vec2 orientation) {
	this->position = position;
	this->orientation = orientation;

	this->update_view(orientation);
}

void Camera::update_projection(int width, int height, float fov) {
	this->priv.projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.01f, 1000.0f);
}

void Camera::update_view(glm::vec2 orientation) {
	this->orientation = orientation;

	// float sinx = glm::sin(orientation[0]);
	// float cosx = glm::sin(orientation[0]);
	// float siny = glm::sin(orientation[1]);
	float cosy = glm::cos(orientation.y);

	glm::vec3 forward(
		glm::sin(orientation.x) * cosy,
		glm::sin(orientation.y),
		glm::cos(orientation.x) * cosy
	);

	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(right, forward);

	this->priv.view = glm::lookAt(position, position + forward, up);
	this->priv.view_skybox = glm::mat4(glm::mat3(this->priv.view));
}

void Camera::move(float input[], float deltatime) {
	float sinx = glm::sin(this->orientation.x);
	float cosx = glm::cos(this->orientation.x);
	// float siny = glm::sin(this->orientation[1]);
	float cosy = glm::cos(this->orientation.y);

	glm::vec3 forward(
		sinx * cosy,
		glm::sin(this->orientation.y),
		cosx * cosy
	);

	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(right, forward);

	glm::vec3 movement_right(-cosx, 0.0, sinx);
	this->position = this->position + 20.f * deltatime * glm::vec3(forward * input[1] + movement_right * input[0]);

	this->priv.view = glm::lookAt(this->position, this->position + forward, up);
	this->priv.view_skybox = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), forward, up);
}

void Camera::computeMatricies() {
	this->priv.camera = this->priv.projection * this->priv.view ;
	this->priv.camera_skybox = this->priv.projection * this->priv.view_skybox;
	glm::vec4 unit(1.0f, 1.0f, 1.0f, 1.0f);
}

const CameraPrivates& Camera::read() {
	return this->priv;
}
