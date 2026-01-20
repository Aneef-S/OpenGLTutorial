#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Mouse Motion
	float fov = 45.0f;
	bool firstMouse = true;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float yaw = -90.0f;
	float pitch = 0.0f;


	float cameraSpeed = 10.0f;


	

	void HandleMouse(GLFWwindow* window, double xpos, double ypos);
	void HandleScroll(GLFWwindow* window, double xoffset, double yoffset);


public:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	Camera(int speed = 10, glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, -3.0f))
		: cameraSpeed(speed), cameraPosition(initialPosition), firstMouse(true) {};


	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	void HandleInput(GLFWwindow* window, float deltaTime);

	void SetLastPosition(float lastx, float lasty);
};

