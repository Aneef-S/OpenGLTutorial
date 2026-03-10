#include "Renderer.h"

void Renderer::Render()
{
    shader->use();

   

    shader->setVec3f("viewPos",scene->camera.GetPosition());
    shader->setMat4f("projection",scene->camera.GetProjection());
    shader->setMat4f("view",scene->camera.GetView());

    for(auto& entity : scene->entities)
    {
        shader->setMat4f("model",entity.transform);
        entity.Draw(*shader);
    }

    for(int i=0;i<scene->dirLights.size();i++)
    {
       scene->dirLights[i].AddToShader(*shader, std::to_string(i));
    }
    

    glfwSwapBuffers(window->window);
	glfwPollEvents();
}


void Renderer::SetScene(Scene & scene)
{
	this->scene = &scene;
    glfwSetWindowUserPointer(window->window,&scene.camera);
   
}

void Renderer::SetShader(Shader & shader)
{
    this->shader = &shader;
}

void Renderer::SetWindow(Window & window)
{
    this->window = &window;
}
