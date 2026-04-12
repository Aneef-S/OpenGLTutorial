#include "Renderer.h"

void Renderer::Render() const
{

    shader->use();

    for (auto &entity : scene->entities)
    {
        shader->setMat4f("model", entity.transform);
        entity.Draw(*shader);
    }

}

void Renderer::AddLights()
{
    shader->use();
    for (int i = 0; i < scene->dirLights.size(); i++)
    {
        scene->dirLights[i].AddToShader(*shader, std::to_string(i));
    }
 
}

void Renderer::ResetCameraValues()
{
    shader->use();
    shader->setUniformBlock("Matrices", 0);
}

void Renderer::SetScene(Scene &scene)
{
    this->scene = &scene;
    glfwSetWindowUserPointer(window->window, &scene.camera);
}

void Renderer::SetShader(Shader &shader)
{
    this->shader = &shader;
}

void Renderer::SetWindow(Window &window)
{
    this->window = &window;
}
