#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"

glm::vec3 front;
float fov = 45;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput
(GLFWwindow* window
);



int temp() {

   
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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



	//----------------------------------- Transformation ---------------------------------------------//

	

	//----------------------------------- End of Transformation ---------------------------------------------//


	//-----------------------------------Shader Compilation---------------------------------------------//

	Shader shaderProgram("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");


	


	//------------------ Set up vertex data and buffers and configure vertex attributes ------------------//
	

	//-------------------TEXTURE COORDINATES ---------------------//




	unsigned int texture1;
	glGenTextures(1, &texture1);


	glBindTexture(GL_TEXTURE_2D, texture1);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);



	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);


	unsigned int texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);


	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	

	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);


	//----------------END OF TEXTURE COORDINATES ------------------//

	//Components
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};


	float verticesForCube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	Camera camera;
	glfwSetWindowUserPointer(window, &camera);


	//For Element Buffer Object, stores the indices of the vertices rather than the vertices themselves
	unsigned int indices[] =
	{
		0, 1, 3, // First Triangle
		1,2,3 // Second Triangle
	};

	//Vertex Buffer Object
	unsigned int VBO,VAO,EBO;
	//Generate Vertex Array Object ID
	glGenVertexArrays(1, &VAO);
	//Generate buffer ID
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);


	//Gl_array_Buffer is for vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Copy vertices data to buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//Copy indices data to buffer's memory
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	


	//Specify how OpenGL should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//Enable the vertex attribute at location 0
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	


	unsigned int VBO1, VAO1,EBO1;

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);


	glBindVertexArray(VAO1);


	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// Vertices for cube
	unsigned int VBO_cube, VAO_cube;

	glGenVertexArrays(1, &VAO_cube);
	glGenBuffers(1, &VBO_cube);

	glBindVertexArray(VAO_cube);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesForCube), verticesForCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	





	glm::vec3 cubePositions[] = {
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


	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f;





	shaderProgram.use(); 
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);


	//unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram.ID, "projection");


	
	glEnable(GL_DEPTH_TEST);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	while (!glfwWindowShouldClose(window))
	{ 
		// input
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);
		camera.HandleInput(window, deltaTime);

		//------------------ render------------------//
		//Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//Change color dynamically
		float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocaiton = glGetUniformLocation(shaderProgram, "vertexColor");

		



		//Render the object
		float offsetValue = (sin(timeValue)/2) ;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		shaderProgram.use();



		// Smile face

		

		//Object transformation locally
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		////Object transformation globally
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		////Projection transformation
		//glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


		//shaderProgram.setFloat("opacity", 1);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//shaderProgram.setFloat("offset", offsetValue);


		// Set the vertex color using uniform
		//glUniform4f(vertexColorLocaiton, 0.0f, greenValue, 0.0f, 1.0f);

		

		

		


		//glBindVertexArray(VAO);
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////glUseProgram(shaderProgramYellow);
		////glBindVertexArray(VAO);
		////glDrawArrays(GL_TRIANGLES, 0, 3);
		//////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);




		////Container box


		//glm::mat4 trans1 = glm::mat4(1.0f);
		//trans1 = glm::mat4(1.0f);
		//trans1 = glm::translate(trans1, glm::vec3(0.0f, 0.0f, 0.5f));
		//trans1 = glm::rotate(trans1, timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
		//trans1 = glm::scale(trans1, glm::vec3(0.5f, 0.5f, 1.0f));


		//shaderProgram.setFloat("opacity", 0);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans1));


		//glBindVertexArray(VAO1);
		////// type, start, no of vertices
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT ,0);
		//glBindVertexArray(0);

		////glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

		

		// Draw Cube




		


		//Object transformation globally
		const float radius = 15.0f;

		glm::mat4 viewCube;

		viewCube = camera.GetView();

		//Projection transformation
		glm::mat4 projectionCube = glm::mat4(1.0f);
		projectionCube = camera.GetProjection();

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewCube));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionCube));

		glBindVertexArray(VAO_cube);


		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 modelCube = glm::mat4(1.0f);


			modelCube = glm::translate(modelCube, cubePositions[i]);
			//modelCube = glm::scale(modelCube, glm::vec3(0.5f, 0.5f, 1.0f));
			float angle = 20.0f * i;
			//modelCube = glm::rotate(modelCube,  timeValue, glm::vec3(1.0f, i * 0.1f, 0.5f));
			//modelCube = glm::rotate(modelCube, timeValue , glm::vec3(i * 0.1f, 1.0f, 0.5f));

		

			shaderProgram.setFloat("opacity", 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCube));
	

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glfwSwapBuffers(window);
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

