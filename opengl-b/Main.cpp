/*
* MYY702
* Grafika Ypologiston & Sistimata Allilepidrasis
* Askisi 1-B
* 
* Omada:
* Antoniou Christodoulos AM: 2641
* Tsiouri Angeliki AM: 3354
*/


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include "controls.hpp"


GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

// Call srand once to get different random numbers each time we run the program
void initRandomGenerator() {
	srand((unsigned)time(NULL));
}

float generateRandomFloat(float LOW, float HIGH) {
	return LOW + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HIGH - LOW)));
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Πυραμίδα", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture keys being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	initRandomGenerator();
	float HEIGHT = generateRandomFloat(2.0f, 10.0f);
	setModelCenter(HEIGHT / 2.0f);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// Our pyramid model has a square base with 2 triangles and 4 additional faces with 1 triangle each
	// so we have a total of: 2 + 4 = 6 triangles and 6 * 3 = 18 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		// triangle 1 (square base)
		2.5f, 2.5f, 0.0f,
		2.5f, -2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,

		 // triangle 2 (square base)
		2.5f, -2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,
		-2.5f, -2.5f, 0.0f,

		// triangle 3
		0.0f, 0.0f, HEIGHT,
		2.5f, 2.5f, 0.0f,
		2.5f, -2.5f, 0.0f,

		// triangle 4
		0.0f, 0.0f, HEIGHT,
		2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,

		// triangle 5
		0.0f, 0.0f, HEIGHT,
		-2.5f, 2.5f, 0.0f,
		-2.5f, -2.5f, 0.0f,

		// triangle 6
		0.0f, 0.0f, HEIGHT,
		2.5f, -2.5f, 0.0f,
		-2.5f, -2.5f, 0.0f,
	};

	// One color for each vertex. Randomly generated.
	static const GLfloat g_color_buffer_data[] = {
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),

		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),

		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),

		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),

		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),

		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
		generateRandomFloat(0, 1), generateRandomFloat(0, 1), generateRandomFloat(0, 1),
	};

	// Vertex Buffer Object
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Color Buffer Object
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model = glm::mat4(1.0);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard input
		computeMatricesFromInputs();
		Projection = getProjectionMatrix();
		View = getViewMatrix();
		Model = getModelMatrix();
		glm::mat4 MVP = Projection * View * Model;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 3*6); // 3 vertices * 6 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the <Q> key was pressed or the window was closed
	while (!isCapitalKeyPressed(GLFW_KEY_Q) &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

