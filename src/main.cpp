


#include "Window.h"
#include "Renderer.h"
#include "Input.hpp"
#include "Cube.h"

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

    // Entity entity = Entity(model);
    Entity cEntity1 = Entity(cube1);
    Entity cEntity2 = Entity(cube2);
    Entity floorEntity = Entity(floor);

public:
    void Initialize()
    {
        // glDepthFunc(GL_ALWAYS);

        cube1.SetTexture(ASSETS_PATH "textures/container.jpg");
        cube2.SetTexture(ASSETS_PATH "textures/container2.png");
        floor.SetTexture(ASSETS_PATH "textures/floor.jpg");


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
        glEnable(GL_STENCIL_TEST);
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


            glStencilMask(0x00); 
            renderer.SetShader(shader);
            renderer.Render();


            scene.entities.clear();


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

            glStencilFunc(GL_ALWAYS, 1, 0xFF); 
            glStencilMask(0xFF); 
            renderer.SetShader(shader);
            renderer.Render();

            scene.entities.clear();


            // Render the outline of the cubes, which will only be drawn where stencil buffer values are not 1 (i.e., around the cubes)


            model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(20.0f * currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.05f));
            cEntity1.transform = model;
            scene.entities.emplace_back(cEntity1);

   
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-20.0f * currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.05f));
            cEntity2.transform = model;
            scene.entities.emplace_back(cEntity2);

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); 
            glDisable(GL_DEPTH_TEST);
            renderer.SetShader(singleColorShader);
            renderer.InitializeRendererValues();
            renderer.Render();

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);   
            glEnable(GL_DEPTH_TEST);  

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
