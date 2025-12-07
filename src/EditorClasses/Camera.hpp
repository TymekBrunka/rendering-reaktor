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
	float orientation_x;
	float orientation_y;
	glm::vec3 position;

	Camera(glm::vec3 position, float orientation_x, float orientation_y);
	void update_projection(int width, int height, float fov);
	void update_view(glm::vec3 position, float orientation_x, float orientation_y);
	void move_and_set_orientation(float input[], float deltatime, float orientation_x, float orientation_y);
	void computeMatricies();
	const CameraPrivates& read();
};
