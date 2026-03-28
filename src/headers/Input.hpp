#ifndef INPUT_H
#define INPUT_H

#include "Window.h"
#include "Camera.hpp"

class Input
{
private:
    Window* window;
public:
    Input(Window &window) : window(&window) {

        glfwSetScrollCallback(window.window, Input::processScrollInput);
        glfwSetCursorPosCallback(window.window, Input::processMouseInput);
    }
    void processKeyInput();
    static void processMouseInput(GLFWwindow* window, double xpos, double ypos);
    static void processScrollInput(GLFWwindow* window, double xoffset, double yoffset);
};



#endif