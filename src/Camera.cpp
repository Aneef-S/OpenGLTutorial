#include "Camera.hpp"

glm::mat4 Camera::GetView() const
{
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	return view;
}

glm::mat4 Camera::GetProjection() const
{
	glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	return projection;
}

glm::vec3 Camera::GetPosition() const
{
    return this->cameraPosition;
}

void Camera::HandleInput(GLFWwindow* window, float deltaTime)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition += deltaTime * cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition -= deltaTime * cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * cameraSpeed;
	}

}

void Camera::SetLastPosition(float lastx, float lasty)
{
	lastX = lastx;
	lastY = lasty;
}

void Camera::LookBackward()
{
    yaw += 180.0f; // Turn around 180 degrees
    
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
}

void Camera::LookForward()
{
    yaw -= 180.0f; // Turn back to the original direction
    
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
}
void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->HandleMouse(window, xpos, ypos);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->HandleScroll(window, xoffset, yoffset);
}

void Camera::HandleMouse(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraFront);
}

void Camera::HandleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
