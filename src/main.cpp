

#include "Window.h"
#include "Renderer.h"
#include "Input.hpp"
#include "Cube.h"
#include "Plane.h"

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

    unsigned int framebuffer;
    unsigned int textureColorBuffer;
    unsigned int rbo; // render buffer object
    unsigned int quadVAO, quadVBO;
    unsigned int mirrorVAO, mirrorVBO;
    unsigned int mirrorFramebuffer;
    unsigned int mirrorTextureColorBuffer;
    unsigned int mirrorRBO;

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


        //Quad for mirror
        glGenVertexArrays(1, &mirrorVAO);
        glGenBuffers(1, &mirrorVBO);
        glBindVertexArray(mirrorVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mirrorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mirrorQuadVertices), &mirrorQuadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glGenFramebuffers(1, &mirrorFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, mirrorFramebuffer);

        // generting and attaching a texture color buffer to the mirror frame buffer
        glGenTextures(1, &mirrorTextureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, mirrorTextureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureColorBuffer, 0);

        // generating and binding render buffer object
        glGenRenderbuffers(1, &mirrorRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, mirrorRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, mirrorRBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mirrorRBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        screenShader.use();
        screenShader.setInt("screenTexture", 0);



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

            glBindFramebuffer(GL_FRAMEBUFFER, mirrorFramebuffer);
            glEnable(GL_DEPTH_TEST);
            glClearColor(50 / 255.0f, 25 / 255.0f, 0 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.camera.LookBackward();
            renderer.ResetCameraValues();
            renderer.Render();
            scene.camera.LookForward();

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

            //rendering the mirror quad
            glBindVertexArray(mirrorVAO);
            glBindTexture(GL_TEXTURE_2D, mirrorTextureColorBuffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window.window);
            glfwPollEvents();
        }

        glDeleteBuffers(1,&quadVBO);
        glDeleteVertexArrays(1,&quadVAO);
        glDeleteRenderbuffers(1,&rbo);
        glDeleteFramebuffers(1,&framebuffer);

        glDeleteBuffers(1, &mirrorVBO);
        glDeleteVertexArrays(1, &mirrorVAO);
        glDeleteRenderbuffers(1, &mirrorRBO);
        glDeleteFramebuffers(1, &mirrorFramebuffer);

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
