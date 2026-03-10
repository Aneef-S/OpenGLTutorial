
#define STB_IMAGE_IMPLEMENTATION

#include "Renderer.h"
#include "Window.h"
#include "Input.hpp"

class Application
{
private:
    Window window;
    Renderer renderer = Renderer(window);
    Input inputHandler = Input(&window);

    Scene scene;

    

    Model model = Model( ASSETS_PATH "models/backpack/backpack.obj");
    Shader shader = Shader(SHADERS_PATH "SimpleVertexShader.glsl", SHADERS_PATH "SimpleFragmentShader.glsl");
    DirectionLight dirLight = DirectionLight();

    Entity entity = Entity(model);

public:
    void Initialize()
    {
        entity.transform = glm::scale(entity.transform, glm::vec3(1.0f, 1.0f, 1.0f));
        scene.entities.push_back(entity);

        dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        dirLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
        dirLight.color = glm::vec3(225 / 255.0f, 128 / 255.0f, 29 / 255.0f);

        scene.dirLights.push_back(dirLight);

        renderer.SetScene(scene);
        renderer.SetShader(shader);
    }

    void Run()
    {
        while (!glfwWindowShouldClose(window.window))
        {
            float currentFrame = glfwGetTime();
            static float lastFrame = 0.0f;
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

           
            inputHandler.processKeyInput();
            scene.camera.HandleInput(window.window, deltaTime);

            glClearColor(50 / 255.0f, 25 / 255.0f, 0 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(20.0f * currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
            scene.entities[0].transform = model;
            

            renderer.Render();
        }

        glfwTerminate();
    }
};

int main()
{
    Application app;
    app.Initialize();
    app.Run();

    return 0;
}
