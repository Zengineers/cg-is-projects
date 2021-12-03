/*
* MYY702
* Grafika Ypologiston & Sistimata Allilepidrasis
* Askisi 1-C
*
* Omada:
* Antoniou Christodoulos AM: 2641
* Tsiouri Angeliki AM: 3354
*/


// Include standard headers
#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


#include "controls.hpp"
#include "objloader.hpp"


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


bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}


void createTexture(const char* texFile, int TEXTURE, const GLuint& texID)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texFile, &width, &height, &nrChannels, 0);

	if (!data)
		std::cout << "Failed to load textures" << std::endl;

	// Sun texture
	glActiveTexture(TEXTURE);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}


void loadObject(
	const char* objFile,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	GLuint& vertexbuffer, 
	GLuint& uvbuffer)
{
	// Read our.obj file
	bool res = loadOBJ(objFile, vertices, uvs, normals);

	if (!res)
		std::cout << "Failed to load object" << std::endl;

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}


void applyAttribute(int attribute, const GLuint& buffer, int size)
{
	glEnableVertexAttribArray(attribute);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		attribute,                  // attribute number, must match the layout in the shader.
		size,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}


void selectTexture(GLuint& TextureID, const GLuint& programID, const GLuint& texID, const GLint& unit)
{
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glBindTexture(GL_TEXTURE_2D, texID);

	// Set our "myTextureSampler" sampler to use the given Texture Unit 
	glUniform1i(TextureID, unit);
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
	window = glfwCreateWindow(800, 800, "Ηλιακό Σύστημα", NULL, NULL);

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
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	// Ensure we can capture keys being pressed below
	glClearColor(0.04f, 0.02f, 0.08f, 1.0f); // Set background color as black
	glEnable(GL_DEPTH_TEST);	// Enable depth test
	glDepthFunc(GL_LESS);	// Accept fragment if it closer to the camera than the former one
	glEnable(GL_CULL_FACE);	// Cull triangles which normal is not towards the camera

	// Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Load textures
	GLuint textureID[6];
	glGenTextures(6, textureID);

	createTexture("sun.jpg", GL_TEXTURE0, textureID[0]);
	createTexture("planet.jpg", GL_TEXTURE1, textureID[1]);
	createTexture("meteor.jpg", GL_TEXTURE2, textureID[2]);
	createTexture("neptune.jpg", GL_TEXTURE3, textureID[3]);
	createTexture("earth.jpg", GL_TEXTURE4, textureID[4]);
	createTexture("mars.jpg", GL_TEXTURE5, textureID[5]);

	glGenerateMipmap(GL_TEXTURE_2D);


	// Vertex and UV Buffer Objects
	GLuint sunVertexBuffer, sunUVBuffer;		
	vector<vec3> sunVertices;
	vector<vec3> sunNormals;
	vector<vec2> sunUVs;
	loadObject("sun.obj", sunVertices, sunUVs, sunNormals, sunVertexBuffer, sunUVBuffer);

	// Vertex and UV Buffer Objects
	GLuint planetVertexBuffer, planetUVBuffer;
	vector<vec3> planetVertices;
	vector<vec3> planetNormals;
	vector<vec2> planetUVs;
	loadObject("planet.obj", planetVertices, planetUVs, planetNormals, planetVertexBuffer, planetUVBuffer);

	// Vertex and UV Buffer Objects
	GLuint meteorVertexBuffer, meteorUVBuffer;
	vector<vec3> meteorVertices;
	vector<vec3> meteorNormals;
	vector<vec2> meteorUVs;
	loadObject("meteor.obj", meteorVertices, meteorUVs, meteorNormals, meteorVertexBuffer, meteorUVBuffer);


	GLuint TextureID;
	mat4 MVP;
	mat4 Projection;
	mat4 View;
	mat4 Model;

	setInitialMatrices();

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard input
		computeMatrices();
		Projection = getProjectionMatrix();
		View = getViewMatrix();
		Model = getSunModelMatrix();
		MVP = Projection * View * Model;


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		selectTexture(TextureID, programID, textureID[0], 0);
		applyAttribute(0, sunVertexBuffer, 3);
		applyAttribute(1, sunUVBuffer, 2);
		glDrawArrays(GL_TRIANGLES, 0, sunVertices.size());

		if (!meteorCollidedWithPlanet()) {
			Model = getPlanetModelMatrix();
			MVP = Projection * View * Model;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			selectTexture(TextureID, programID, textureID[1], 1);
			applyAttribute(0, planetVertexBuffer, 3);
			applyAttribute(1, planetUVBuffer, 2);
			glDrawArrays(GL_TRIANGLES, 0, planetVertices.size());
		}

		if (!meteorCollidedWithNeptune()) {
			Model = getNeptuneModelMatrix();
			MVP = Projection * View * Model;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			selectTexture(TextureID, programID, textureID[3], 3);
			applyAttribute(0, planetVertexBuffer, 3);
			applyAttribute(1, planetUVBuffer, 2);
			glDrawArrays(GL_TRIANGLES, 0, planetVertices.size());
		}

		if (!meteorCollidedWithEarth()) {
			Model = getEarthModelMatrix();
			MVP = Projection * View * Model;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			selectTexture(TextureID, programID, textureID[4], 4);
			applyAttribute(0, planetVertexBuffer, 3);
			applyAttribute(1, planetUVBuffer, 2);
			glDrawArrays(GL_TRIANGLES, 0, planetVertices.size());
		}

		if (!meteorCollidedWithMars()) {
			Model = getMarsModelMatrix();
			MVP = Projection * View * Model;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			selectTexture(TextureID, programID, textureID[5], 5);
			applyAttribute(0, planetVertexBuffer, 3);
			applyAttribute(1, planetUVBuffer, 2);
			glDrawArrays(GL_TRIANGLES, 0, planetVertices.size());
		}

		if (meteorActive()) {
			Model = getMeteorModelMatrix();
			MVP = Projection * View * Model;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			selectTexture(TextureID, programID, textureID[2], 2);
			applyAttribute(0, meteorVertexBuffer, 3);
			applyAttribute(1, meteorUVBuffer, 2);
			glDrawArrays(GL_TRIANGLES, 0, meteorVertices.size());
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the <Q> key was pressed or the window was closed
	while (!isCapitalKeyPressed(GLFW_KEY_Q) &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &sunVertexBuffer);
	glDeleteBuffers(1, &sunUVBuffer);
	glDeleteBuffers(1, &planetVertexBuffer);
	glDeleteBuffers(1, &planetUVBuffer);
	glDeleteBuffers(1, &meteorVertexBuffer);
	glDeleteBuffers(1, &meteorUVBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glDeleteTextures(6, textureID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



