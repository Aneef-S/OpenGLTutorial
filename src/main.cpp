

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
    1.0f, 1.0f, 1.0f, 1.0f};

float points[] = {
    -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top-right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
};

class Application
{
private:
    Window window;
    Renderer renderer = Renderer(window);
    Input inputHandler = Input(window);

    Scene scene;

    Model backpackModel = Model(ASSETS_PATH "models/backpack/backpack.obj");
    Model planetModel = Model(ASSETS_PATH "models/planet/planet.obj");
    Model rockModel = Model(ASSETS_PATH "models/rock/rock.obj");

    Shader shader = Shader(
        SHADERS_PATH "SimpleVertexShader.glsl",
        SHADERS_PATH "SimpleFragmentShader.glsl");

    Shader instanceShader = Shader(
        SHADERS_PATH "InstanceVertexShader.glsl",
        SHADERS_PATH "SimpleFragmentShader.glsl");

    Shader screenShader = Shader(
        SHADERS_PATH "ScreenVertexShader.glsl",
        SHADERS_PATH "ScreenFragmentShader.glsl");

    unsigned int framebuffer;
    unsigned int textureColorBuffer;
    unsigned int rbo; // render buffer object
    unsigned int quadVAO, quadVBO;

    unsigned int uboMatrices;

    glm::mat4 *modelMatrices;

    unsigned int amount = 100000;

public:
    void Initialize()
    {

        renderer.SetScene(scene);
        renderer.SetShader(shader);

        modelMatrices = new glm::mat4[amount];
        srand(glfwGetTime()); // initialize random seed
        float radius = 50.0;
        float offset = 10.0f;
        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: scale between 0.05 and 0.25f
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            modelMatrices[i] = model;
        }

        // vertex buffer object for instanced array positions
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        // set transformation matrices as an instance vertex attribute (with divisor 1)
        for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
        {
            unsigned int VAO = rockModel.meshes[i].VAO;
            glBindVertexArray(VAO);
            // vertex attributes
            std::size_t vec4Size = sizeof(glm::vec4);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(1 * vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }

        // Quad for the screen
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

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

        shader.use();
        shader.setUniformBlock("Matrices", 0);
        instanceShader.use();
        instanceShader.setUniformBlock("Matrices", 0);
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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model = glm::mat4(1.0f);

            renderer.SetShader(shader);
            renderer.AddLights();
            renderer.ResetCameraValues();
            scene.entities.clear();

            // scene.entities.push_back(Entity(backpackModel, model));

            glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(scene.camera.GetView()));
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(scene.camera.GetProjection()));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            renderer.SetShader(shader);
            renderer.Render();

            // Drawing the planet
            shader.use();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
            shader.setMat4f("model", model);
            planetModel.Draw(shader);

            // draw meteorites
            instanceShader.use();
            instanceShader.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rockModel.textures_loaded[0].id); 
            for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
            {
                glBindVertexArray(rockModel.meshes[i].VAO);
                glDrawElementsInstanced(
                    GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            }

            scene.entities.clear();

            // rendering from the texture color buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            // glClearColor(66.0f/255.0f, 66.0f/255.0f, 66.0f/255.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
            glClear(GL_COLOR_BUFFER_BIT);
            screenShader.use();
            glBindVertexArray(quadVAO);
            glBindTexture(GL_TEXTURE_2D, textureColorBuffer); // use the color attachment texture as the texture of the quad plane
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window.window);
            glfwPollEvents();
        }

        glDeleteBuffers(1, &quadVBO);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteRenderbuffers(1, &rbo);
        glDeleteFramebuffers(1, &framebuffer);

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
