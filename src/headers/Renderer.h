#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Scene.h"
#include "Shader.h"
#include "Window.h"

class Renderer
{
private:
    Scene *scene;
    Shader *shader;
    Window *window;

public:
    void Render();
    Renderer(Window &window) : window(&window)
    {
        glEnable(GL_DEPTH_TEST);
        stbi_set_flip_vertically_on_load(true);
    }
    void SetScene(Scene &scene);
    void SetShader(Shader &shader);
    void SetWindow(Window &window);
};

#endif // RENDERER_H