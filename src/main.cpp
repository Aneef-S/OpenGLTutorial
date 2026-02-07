#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(char const* path);
void processInput
(GLFWwindow* window
);




int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL_RENDERER", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	glEnable(GL_DEPTH_TEST);


	Shader shaderProgram("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	Shader lightShaderProgram("src/Shaders/VertexShader.glsl", "src/Shaders/LightFragmentShader.glsl");

	Camera camera{};

	glfwSetWindowUserPointer(window, &camera);


	unsigned int diffuseMap = loadTexture("resources/textures/container2.png");
	unsigned int specularMap = loadTexture("resources/textures/container2_specular.png");
	unsigned int emissionMap = loadTexture("resources/textures/emission.jpg");
	

	stbi_set_flip_vertically_on_load(true);

	const float minusPointFive = -0.5f;
	const float plusPointFive = 0.5f;


	float  rectangleVertices[] =
	{
		minusPointFive,minusPointFive,0.0f,
		minusPointFive,plusPointFive,0.0f,
		plusPointFive,plusPointFive,0.0f,
		plusPointFive,minusPointFive,0.0f
	};

	unsigned int rectangleIndices[] =
	{
		0,1,3,
		1,2,3
	};

	float cubeVertices[] =
	{
		minusPointFive,plusPointFive,plusPointFive, // Top Front Left
		minusPointFive,plusPointFive,minusPointFive, // Top Back Left
		plusPointFive,plusPointFive,minusPointFive, // Top Back Right
		plusPointFive,plusPointFive, plusPointFive, // Top Front Right

		minusPointFive,minusPointFive,plusPointFive, // Bottom Front Left
		minusPointFive,minusPointFive,minusPointFive, // Bottom Back Left
		plusPointFive,minusPointFive,minusPointFive, // Bottom Back Right
		plusPointFive,minusPointFive,plusPointFive // Bottom Front Right


	};

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};



	unsigned cubeIndices[] = {
		0,3,7,
		0,4,7, // Front

		0,4,5,
		0,1,5, // Right

		1,5,6,
		1,2,6, // Back

		3,7,6,
		3,2,6, // Right

		3,0,1,
		3,2,1, // Top

		4,7,6,
		4,5,6, // Bottom

	};


	unsigned int cube_VBO, cube_VAO;

	glGenVertexArrays(1, &cube_VAO);
	glGenBuffers(1, &cube_VBO);


	glBindVertexArray(cube_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	unsigned int rectangle_VBO, rectangle_VAO, rectangle_EBO;

	glGenVertexArrays(1, &rectangle_VAO);
	glGenBuffers(1, &rectangle_VBO);
	glGenBuffers(1, &rectangle_EBO);

	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	double startX, startY;
	glfwGetCursorPos(window, &startX, &startY);
	//camera.SetLastPosition(startX, startY);

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	glm::vec4 lightColor = glm::vec4(0.8f, 0.8f, 0.7f, 1.0f);
	glm::vec4 color = glm::vec4(0.2f, 0.2f, 0.0f, 1.0f);


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


	
	float shininess = 64.0f;


	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

	const glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	shaderProgram.use();
	shaderProgram.setInt("material.diffuse", 0);
	shaderProgram.setInt("material.specular", 1);
	shaderProgram.setInt("material.emission", 2);

	shaderProgram.setFloat("material.shininess", shininess);
	shaderProgram.setFloat("material.emissionStrength", 0.4f);
	shaderProgram.setVec3f("light.ambient", lightAmbient);
	shaderProgram.setVec3f("light.diffuse", lightDiffuse);
	shaderProgram.setVec3f("light.specular", lightSpecular);

	glm::vec3 lightPos = glm::vec3(0.0f,0.0f,-2.0f);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera.HandleInput(window,deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		view = camera.GetView();
		projection = camera.GetProjection();

		
		
		lightColor = glm::vec4(glm::sin(currentFrame) * 0.5f + 0.5f, glm::sin(currentFrame / 2) * 0.5f + 0.5f, glm::cos(currentFrame) * 0.5f + 0.5f, 1.0f);
		

	
		shaderProgram.use();

		model = glm::mat4(1.0f);
	



		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			model = glm::rotate(model,glm::radians(i * 20.0f),glm::vec3(1.0f,3.0f,.5f));

			shaderProgram.setMat4f("model", model);
			shaderProgram.setMat4f("view", view);
			shaderProgram.setMat4f("projection", projection);
			shaderProgram.setVec3f("light.lightPosition", lightPos);
			shaderProgram.setVec4f("light.color", lightColor);
			shaderProgram.setVec3f("viewPos", camera.cameraPosition);
			shaderProgram.setFloat("material.emissionStrength", abs(sin(currentFrame))*0.4f);

			glBindVertexArray(cube_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);



		}


		
		


		lightShaderProgram.use();

		

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);

		
		

		lightShaderProgram.setMat4f("model", model);
		lightShaderProgram.setMat4f("view", view);
		lightShaderProgram.setMat4f("projection", projection);
		lightShaderProgram.setVec4f("color", lightColor);

		glBindVertexArray(cube_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		




		
		glfwSwapBuffers(window);
		glfwPollEvents();

		
	}


	glfwTerminate();


	return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(
	GLFWwindow* window
)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera::mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::scroll_callback(window, xoffset, yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}