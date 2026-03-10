#include "Input.hpp"

void Input::processKeyInput()
{
    if (glfwGetKey(this->window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window->window, true);
}

void Input::processMouseInput(GLFWwindow * window, double xpos, double ypos)
{
    Camera::mouse_callback(window, xpos, ypos);
}

void Input::processScrollInput(GLFWwindow * window, double xoffset, double yoffset)
{
    Camera::scroll_callback(window, xoffset, yoffset);
}


