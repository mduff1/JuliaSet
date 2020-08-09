#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include <shaders/shaders.h>


// settings
unsigned int scr_height = 1080;
unsigned int scr_width = (16.0 / 9.0) * scr_height;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}



float vertices[] = {
	// positions       
	-1.0f, -1.0f, 0.0f, // bottom left
	 1.0f,  1.0f, 0.0f, // top right
	-1.0f,  1.0f, 0.0f, // top left
	 1.0f, -1.0f, 0.0f  // bottom right
};


unsigned int indices[] =
{
	0, 1, 2,
	0, 3, 1
};




int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	

	GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Julia Set", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader plot("C:/Programming/C++/OpenGL Projects/JuliaSet/JuliaSet/shaders/vertexS.txt", "C:/Programming/C++/OpenGL Projects/JuliaSet/JuliaSet/shaders/fragmentS.txt");

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		
		 
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		int sWidth, sHeight;
		glfwGetFramebufferSize(window, &sWidth, &sHeight);
		glm::vec2 resolution = glm::vec2(sWidth, sHeight);

		

		double cXpos, cYpos;
		glfwGetCursorPos(window, &cXpos, &cYpos);
		glm::vec2 cursorPos = glm::vec2(cXpos, sHeight - cYpos);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		plot.use();
		plot.setVec2("resolution", resolution);
		plot.setVec2("cursorPos", cursorPos);
		plot.setFloat("time", glfwGetTime());
		
		
		// add GUI


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}