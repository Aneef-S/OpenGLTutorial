#ifndef WINODW_H
#define WINODW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
    void initializeWindow();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
    GLFWwindow* window;
    Window() ;
};

#endif // WINODW_H