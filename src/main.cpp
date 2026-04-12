

#include "Window.h"
#include "Renderer.h"
#include "Input.hpp"
#include "Cube.h"
#include "Plane.h"
#include "Utils.h"

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

float mirrorQuadVertices[] = { 
    // positions   // texCoords
    -0.4f, 0.6f,  1.0f, 0.0f, // Bottom-Left
     0.4f, 0.6f,  0.0f, 0.0f, // Bottom-Right
     0.4f,  0.9f,  0.0f, 1.0f, // Top-Right

    -0.4f, 0.6f,  1.0f, 0.0f, // Bottom-Left
    0.4f,  0.9f,  0.0f, 1.0f, // Top-Right
    -0.4f,  0.9f,  1.0f, 1.0f  // Top-Left
};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

class Application
{
private:
    Window window;
    Renderer renderer = Renderer(window);
    Input inputHandler = Input(window);

    Scene scene;

    Model model = Model(ASSETS_PATH "models/backpack/backpack.obj");
    Shader shader = Shader(SHADERS_PATH "SimpleVertexShader.glsl", SHADERS_PATH "SimpleFragmentShader.glsl");
    Shader screenShader = Shader(SHADERS_PATH "ScreenVertexShader.glsl", SHADERS_PATH "ScreenFragmentShader.glsl");
    Shader skyboxShader = Shader(SHADERS_PATH "SkyboxVertexShader.glsl", SHADERS_PATH "SkyboxFragmentShader.glsl");
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

    std::vector<std::string> faces = 
    {
        TEXTURES_PATH "skybox/right.jpg",
        TEXTURES_PATH "skybox/left.jpg",
        TEXTURES_PATH "skybox/top.jpg",
        TEXTURES_PATH "skybox/bottom.jpg",
        TEXTURES_PATH "skybox/front.jpg",
        TEXTURES_PATH "skybox/back.jpg"
    };

    unsigned int cubemapTexture;
    

    unsigned int skyboxVAO, skyboxVBO;

    unsigned int framebuffer;
    unsigned int textureColorBuffer;
    unsigned int rbo; // render buffer object
    unsigned int quadVAO, quadVBO;

    unsigned int uboMatrices;


public:
    void Initialize()
    {
        // glDepthFunc(GL_ALWAYS);

        cube1.SetTexture(TEXTURES_PATH "container.jpg");
        cube2.SetTexture(TEXTURES_PATH "container2.png");
        floor.SetTexture(TEXTURES_PATH "floor.jpg");
        grass.SetTexture(TEXTURES_PATH "blending_transparent_window.png");

        stbi_set_flip_vertically_on_load(false);
        cubemapTexture = Utils::LoadCubemap(faces);
        stbi_set_flip_vertically_on_load(true);

        dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        dirLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        dirLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
        dirLight.color = glm::vec3(200 / 255.0f, 200 / 255.0f, 255 / 255.0f);

        scene.dirLights.emplace_back(dirLight);

        renderer.SetScene(scene);
        renderer.SetShader(shader);


        // Skybox VAO and VBO setup
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



        
        //Quad for the screen
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // creating a color texture buffer.

        // generating and binding the frame buffer
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // generting and attaching a texture color buffer to the frame buffer
        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        // generating and binding render buffer object
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));


        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        shader.use();
        shader.setInt("skybox", 0);
    }

    void Run()
    {
        glEnable(GL_CULL_FACE);
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

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST);
            glClearColor(50 / 255.0f, 25 / 255.0f, 0 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
           

            glm::mat4 model = glm::mat4(1.0f);

            renderer.SetShader(shader);
            renderer.AddLights();
            renderer.ResetCameraValues();
            scene.entities.clear();

            glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(scene.camera.GetView()));
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(scene.camera.GetProjection()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0); 


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

            // Render the grass
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(4.0f, 0.0f, 4.0f));
            grassEntity.transform = model;
            scene.entities.emplace_back(grassEntity);

            renderer.SetShader(shader);
            renderer.Render();

            //rendering skybox last, since we want it to be at the farthest depth
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            skyboxShader.setUniformBlock("Matrices", 0);
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default

            scene.entities.clear();

            // rendering from the texture color buffer
            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
            glClear(GL_COLOR_BUFFER_BIT);
            screenShader.use();
            glBindVertexArray(quadVAO);
            glBindTexture(GL_TEXTURE_2D, textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
            glDrawArrays(GL_TRIANGLES, 0, 6);


            glfwSwapBuffers(window.window);
            glfwPollEvents();
        }

        glDeleteBuffers(1,&quadVBO);
        glDeleteVertexArrays(1,&quadVAO);
        glDeleteRenderbuffers(1,&rbo);
        glDeleteFramebuffers(1,&framebuffer);


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
