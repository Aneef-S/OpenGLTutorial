#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

   
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
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


	//-----------------------------------Shader Compilation---------------------------------------------//

	const char* vertexShaderPath = "Shaders/vertexShader.glsl";
	const char* fragmentShaderPath = "Shaders/fragmentShader.glsl";

	Shader shader = Shader(vertexShaderPath,fragmentShaderPath);


	//------------------ Set up vertex data and buffers and configure vertex attributes ------------------//
	

	//Components
	float vertices[] =
	{
		 0.5f,  0.5f, 0.0f, 
		  0.5f, -0.5f, 0.0f,
		  -0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable the vertex attribute at location 0
	glEnableVertexAttribArray(0);
	


	unsigned int VBO1, VAO1;

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);






	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	

	while (!glfwWindowShouldClose(window))
	{ 
		// input
		processInput(window);

		//------------------ render------------------//
		//Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render the object
		shader.Use();
		glBindVertexArray(VAO1);
		// type, start, no of vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}