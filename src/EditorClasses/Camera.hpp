#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
struct CameraPrivates {
	float fov;
	float pixel_size[2];
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_skybox;
	glm::mat4 camera; //final matrix
	glm::mat4 camera_skybox; //final matrix
};

class Camera {
private:
	CameraPrivates priv;
public:
	bool holding_rmb;
	glm::vec2 orientation;
	glm::vec2 last_mouse_pos;
	glm::vec3 position;

	Camera(glm::vec3 position, glm::vec2 orientation);
	void update_projection(int width, int height, float fov);
	void update_view(glm::vec3 position, glm::vec2 orientation);
	void move_and_set_orientation(float input[], float deltatime, glm::vec2 orientation);
	void computeMatricies();
	const CameraPrivates& read();
};
