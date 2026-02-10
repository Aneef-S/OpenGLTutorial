#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "glad_d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int GetTextureFromImage(const std::string &imagePath);


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//-----------------------------------Shader Compilation---------------------------------------------//

	const char *vertexShaderPath = "Shaders/vertexShader.glsl";
	const char *fragmentShaderPath = "Shaders/fragmentShader.glsl";

	Shader shader = Shader(vertexShaderPath, fragmentShaderPath);

	// Textures

	const char *textureImagePath = "Resources/container.jpg";
	unsigned int containerTextureID = GetTextureFromImage(textureImagePath);
	unsigned int smilyTextureID = GetTextureFromImage("Resources/awesomeface.png");
	
	

	//------------------ Set up vertex data and buffers and configure vertex attributes ------------------//

	// Components
	float vertices[] = {
			// positions          // colors           // texture coords
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,		// top right
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f		// top left
	};

	unsigned int indices[] = {
			0, 1, 2,
			0, 3, 2
		};

	// Vertex Buffer Object
	unsigned int VBO, VAO, EBO;

	// Generate Vertex Array Object ID
	glGenVertexArrays(1, &VAO);
	// Generate buffer ID
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// Gl_array_Buffer is for vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertices data to buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify how OpenGL should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Setting color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setting texture coordinates.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

	shader.Use();
	shader.SetInt("containerTexture", 0);
	shader.SetInt("smilyTexture",1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerTextureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,smilyTextureID);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		float currentTime = glfwGetTime();

		//------------------ render------------------//
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		float sinOfTime = sin(currentTime);
		trans = glm::scale(trans,glm::vec3(sinOfTime,sinOfTime,1.0f));

		shader.Use();
		shader.SetMatrix4fv("transform",trans);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

unsigned int GetTextureFromImage(const std::string &imagePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannles;
	unsigned char *textureData = stbi_load(imagePath.c_str(), &width, &height, &nrChannles, 0);

	if (textureData)
	{
		GLenum format;
		if (nrChannles == 1) format = GL_RED;
		else if (nrChannles == 3) format = GL_RGB;
		else if (nrChannles == 4) format = GL_RGBA;

		glTexImage2D(
			GL_TEXTURE_2D,
			0, 
			format, 
			width, 
			height, 
			0, 
			format, 
			GL_UNSIGNED_BYTE, 
			textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(textureData);
	return textureID;
}