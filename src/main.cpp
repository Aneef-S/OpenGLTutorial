


#include "Window.h"
#include "Renderer.h"
#include "Input.hpp"
#include "Cube.h"
#include "Plane.h"

class Application
{
private:
    Window window;
    Renderer renderer = Renderer(window);
    Input inputHandler = Input(window);

    Scene scene;

    

    Model model = Model( ASSETS_PATH "models/backpack/backpack.obj");
    Shader shader = Shader(SHADERS_PATH "SimpleVertexShader.glsl", SHADERS_PATH "SimpleFragmentShader.glsl");
    Shader singleColorShader = Shader(SHADERS_PATH "SimpleVertexShader.glsl", SHADERS_PATH "SingleColorFragmentShader.glsl");
    DirectionLight dirLight = DirectionLight();

    Cube cube1;
    Cube cube2;
    Cube floor;
    Plane grass;

    // Entity entity = Entity(model);
    Entity cEntity1 = Entity(cube1);
    Entity cEntity2 = Entity(cube2);
    Entity floorEntity = Entity(floor);
    Entity grassEntity = Entity(grass);

public:
    void Initialize()
    {
        // glDepthFunc(GL_ALWAYS);

        cube1.SetTexture(ASSETS_PATH "textures/container.jpg");
        cube2.SetTexture(ASSETS_PATH "textures/container2.png");
        floor.SetTexture(ASSETS_PATH "textures/floor.jpg");
        grass.SetTexture(ASSETS_PATH "textures/blending_transparent_window.png");


        dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        dirLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
        dirLight.color = glm::vec3(225 / 255.0f, 128 / 255.0f, 29 / 255.0f);

        scene.dirLights.emplace_back(dirLight);

        renderer.SetScene(scene);
        renderer.SetShader(shader);

     
        singleColorShader.use();
        singleColorShader.setVec3f("color", glm::vec3(0.5f, 0.5f, 0.5f));

        shader.use();
    }

    void Run()
    {
        glEnable(GL_CULL_FACE);  
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        while (!glfwWindowShouldClose(window.window))
        {
            float currentFrame = glfwGetTime();
            static float lastFrame = 0.0f;
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            inputHandler.processKeyInput();
            scene.camera.HandleInput(window.window, deltaTime);

            glClearColor(50 / 255.0f, 25 / 255.0f, 0 / 255.0f, 1.0f);


            glEnable(GL_DEPTH_TEST);
            
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glm::mat4 model = glm::mat4(1.0f);


            renderer.SetShader(shader);
            renderer.InitializeRendererValues();
            scene.entities.clear();

            // Render the floor first, to set stencil buffer values to 1 where the floor is drawn
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(20.0f, .99f, 20.0f));
            floorEntity.transform = model;
            scene.entities.emplace_back(floorEntity);

            // Render the cubes, which will set stencil buffer values to 1 where the cubes are drawn
            model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(20.0f * currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
            cEntity1.transform = model;
            scene.entities.emplace_back(cEntity1);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-20.0f * currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
            cEntity2.transform = model;
            scene.entities.emplace_back(cEntity2);

            //Render the grass
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(4.0f, 0.0f, 4.0f));
            grassEntity.transform = model;
            scene.entities.emplace_back(grassEntity);

            renderer.SetShader(shader);
            renderer.Render();

            scene.entities.clear();


            glfwSwapBuffers(window.window);
            glfwPollEvents();

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
