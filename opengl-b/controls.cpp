/*
* MYY702
* Grafika Ypologiston & Sistimata Allilepidrasis
* Askisi 1-B
*
* Omada:
* Antoniou Christodoulos AM: 2641
* Tsiouri Angeliki AM: 3354
*/


// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

#include <Windows.h>
#include "controls.hpp"

#define DEBUG_MODE 0

float cameraOrbitSpeed = 1.0f;		// rotation angle degrees
float cameraMoveSpeed = 0.5f;
float FoV = 30.0f;

// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection;
// Camera matrix
glm::mat4 View;
// Model matrix
glm::mat4 Model = glm::mat4(1.0);

glm::mat4 getModelMatrix() {
	return Model;
}
glm::mat4 getViewMatrix() {
	return View;
}
glm::mat4 getProjectionMatrix() {
	return Projection;
}

// Initial camera position
glm::vec3 position = glm::vec3(10.0f, 10.0f, 30.0f);
//glm::vec3 direction = glm::vec3(0, 0, 0);
glm::vec3 up = glm::vec3(0, 0, 1.0f);

glm::vec3 modelCenter = glm::vec3(0, 0, 0);;
glm::vec3 normalizedVector;

// Called from main to pass the model center
void setModelCenter(float center) {
	modelCenter.z = center;
}

int isCapsLockOn()
{
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
		// Caps Lock On
		return 1;
	}
	else {
		// Caps Lock Off
		return 0;
	}
}

// Detect whether key is pressed with caps lock on or left/right shift
int isCapitalKeyPressed(int KEY_CONSTANT)
{
	int CAPS_LOCK_ON = isCapsLockOn();
	int Q_KEY_PRESSED = (glfwGetKey(window, KEY_CONSTANT) == GLFW_PRESS);
	int LEFT_SHIFT_PRESSED = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	int RIGHT_SHIFT_PRESSED = (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

	if (!Q_KEY_PRESSED)
		return 0;

	if (CAPS_LOCK_ON) {

		if (!LEFT_SHIFT_PRESSED && !RIGHT_SHIFT_PRESSED)
			return 1;
	}
	else {

		if (LEFT_SHIFT_PRESSED || RIGHT_SHIFT_PRESSED)
			return 1;
	}

	return 0;
}


void computeMatricesFromInputs(){

	//  Scale up model
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS &&
		!isCapitalKeyPressed(GLFW_KEY_Q)) {
		Model = glm::scale(
			Model,
			glm::vec3(1.03f));
	}

	// Scale down model
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS &&
		!isCapitalKeyPressed(GLFW_KEY_Z)) {
		Model = glm::scale(
			Model,
			glm::vec3(0.97f));
	}

	// Rotate model on Z axis
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		Model = glm::rotate(
			Model,
			glm::radians(3.0f),
			glm::vec3(0, 0, 1));
	}

	// Orbit camera around X axis
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position = glm::rotateX(
			position,
			glm::radians(cameraOrbitSpeed));
	}

	// Orbit camera around X axis
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		position = glm::rotateX(
			position,
			glm::radians(-cameraOrbitSpeed));
	}

	// Orbit camera around Y axis
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position = glm::rotateY(
			position,
			glm::radians(cameraOrbitSpeed));
	}

	// Orbit camera around Y axis
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position = glm::rotateY(
			position,
			glm::radians(-cameraOrbitSpeed));
	}

	// Zoom in
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		normalizedVector = glm::normalize(modelCenter - position);
		position += cameraMoveSpeed * normalizedVector;
	}

	// Zoom out
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		normalizedVector = glm::normalize(modelCenter - position);
		position -= cameraMoveSpeed * normalizedVector;
	}

	// Zoom in by adjusting Projection field of view
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		FoV -= cameraMoveSpeed;
	}

	// Zoom out by adjusting Projection field of view
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		FoV += cameraMoveSpeed;
	}

	// Move camera on Z axis
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		position.z += cameraMoveSpeed;
	}

	// Move camera on Z axis
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		position.z -= cameraMoveSpeed;
	}

	// Reset camera position
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		position = glm::vec3(10.0f, 10.0f, 30.0f);
	}

	if (DEBUG_MODE)
		printf("%f %f %f\n", position.x, position.y, position.z);

	// Update projection matrix
	Projection = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	// Update camera matrix
	View       = glm::lookAt(
								position,           // Camera is here
								modelCenter, // and looks here
								up                  // Head is up
						   );
	
}