/*
* MYY702
* Grafika Ypologiston & Sistimata Allilepidrasis
* Askisi 1-C
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

#define DEBUG_CAMERA 0
#define DEBUG_METEOR 0


float sunOrbitSpeed = -0.1f;
float planetMoveSpeed = 0.5f;
float neptuneMoveSpeed = 0.25f;
float earthMoveSpeed = 0.6f;
float marsMoveSpeed = 0.4f;
float meteorMoveSpeed = 2.0f;
float cameraOrbitSpeed = 1.0f;		// rotation angle degrees
float cameraMoveSpeed = 0.5f;
float sunRadius = 10.0f;
float planetRadius = 5.0f;
float meteorRadius = 2.0f;
float neptuneRadius = 6.5;
float earthRadius = 4.5f;
float marsRadius = 3.75f;
float FoV = 30.0f;
float ratio = 4.0f / 3.0f;
float minDisplayRange = 0.1f;
float maxDisplayRange = 400.0f;

bool isMeteorActive = false;
bool collidedWithPlanet = false;
bool collidedWithSun = false;
bool collidedWithNeptune = false;
bool collidedWithEarth = false;
bool collidedWithMars = false;

 // Projection matrix : 30degrees Field of View, 4:3 ratio, display range : 0.1 unit <-> 400 units
mat4 Projection = perspective(radians(FoV), ratio, minDisplayRange, maxDisplayRange);
mat4 View;	// Camera matrix
mat4 SunModel = mat4(1.0f);	// Sun Model matrix
mat4 PlanetModel = mat4(1.0f);	// Planet Model matrix
mat4 MeteorModel = mat4(1.0f);		// Meteor Model matrix
mat4 NeptuneModel = mat4(1.0f);		// Neptune Model matrix
mat4 EarthModel = mat4(1.0f);		// Earth Model matrix
mat4 MarsModel = mat4(1.0f);		// Mars Model matrix

vec3 cameraPosition = vec3(0.5f, 24.0f, -155.0f);	  // Initial camera position
vec3 cameraUp = vec3(0, 1.0f, 0);
vec3 sunPosition = vec3(0, 0, 0);;
vec3 normalizedVector;
vec4 planetPosition = vec4(25.0f, 0.0f, 0.0f, 1.0f);
vec4 meteorPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 neptunePosition = vec4(-30.0f, 6.0f, 0.0f, 1.0f);
vec4 earthPosition = vec4(-40.0f, -8.0f, 0.0f, 1.0f);
vec4 marsPosition = vec4(65.0f, 4.0f, 0.0f, 1.0f);


vec4 getPlanetPosition() {
	return planetPosition;
}
mat4 getViewMatrix() {
	return View;
}
mat4 getProjectionMatrix() {
	return Projection;
}
mat4 getSunModelMatrix() {
	return SunModel;
}
mat4 getPlanetModelMatrix() {
	return PlanetModel;
}
mat4 getMeteorModelMatrix() {
	return MeteorModel;
}
mat4 getNeptuneModelMatrix() {
	return NeptuneModel;
}
mat4 getEarthModelMatrix() {
	return EarthModel;
}
mat4 getMarsModelMatrix() {
	return MarsModel;
}
bool meteorActive() {
	return isMeteorActive;
}
bool meteorCollidedWithPlanet() {
	return collidedWithPlanet;
}
bool meteorCollidedWithNeptune() {
	return collidedWithNeptune;
}
bool meteorCollidedWithEarth() {
	return collidedWithEarth;
}
bool meteorCollidedWithMars() {
	return collidedWithMars;
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
	int KEY_PRESSED = (glfwGetKey(window, KEY_CONSTANT) == GLFW_PRESS);
	int LEFT_SHIFT_PRESSED = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	int RIGHT_SHIFT_PRESSED = (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

	if (!KEY_PRESSED)
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


// Set the initial transforms of our objects before the render loop
void setInitialMatrices() {
	PlanetModel = translate(PlanetModel, vec3(planetPosition));

	NeptuneModel = scale(NeptuneModel, vec3(1.3f));
	NeptuneModel = translate(NeptuneModel, vec3(neptunePosition));

	EarthModel = scale(EarthModel, vec3(0.9f));
	EarthModel = translate(EarthModel, vec3(earthPosition));

	MarsModel = scale(MarsModel, vec3(0.75f));
	MarsModel = translate(MarsModel, vec3(marsPosition));
}


// Calculate the distance of two spheres and check if it is lesser than the sum of their radiuses
bool detectSphereCollision(vec4 sphere1, vec4 sphere2, float radius1, float radius2) {
	float distance = sqrt((sphere1.x - sphere2.x) * (sphere1.x - sphere2.x) +
								(sphere1.y - sphere2.y) * (sphere1.y - sphere2.y) +
								(sphere1.z - sphere2.z) * (sphere1.z - sphere2.z));
	return (distance < (radius1 + radius2));
}


// Check if the meteor has collided with any other object
void checkForCollisions()
{
	if (!collidedWithPlanet) {
		collidedWithPlanet = detectSphereCollision(meteorPosition, planetPosition, meteorRadius, planetRadius);
		if (collidedWithPlanet)
			isMeteorActive = false;
	}

	if (!collidedWithNeptune) {
		collidedWithNeptune = detectSphereCollision(meteorPosition, neptunePosition, meteorRadius, neptuneRadius);
		if (collidedWithNeptune)
			isMeteorActive = false;
	}

	if (!collidedWithEarth) {
		collidedWithEarth = detectSphereCollision(meteorPosition, earthPosition, meteorRadius, earthRadius);
		if (collidedWithEarth)
			isMeteorActive = false;
	}

	if (!collidedWithMars) {
		collidedWithMars = detectSphereCollision(meteorPosition, marsPosition, meteorRadius, marsRadius);
		if (collidedWithMars)
			isMeteorActive = false;
	}

	collidedWithSun = detectSphereCollision(meteorPosition, vec4(sunPosition, 1.0f), meteorRadius, sunRadius);
	if (collidedWithSun)
		isMeteorActive = false;
}


void transformMeteor()
{
	// Calculate the difference between new and old meteor position
	vec3 newPosition = vec3(meteorPosition);
	normalizedVector = normalize(sunPosition - newPosition);
	newPosition += meteorMoveSpeed * normalizedVector;
	vec3 difference = newPosition - vec3(meteorPosition);

	// Update meteor position
	meteorPosition = vec4(newPosition, 1.0f);

	// Translate meteor model matrix using the difference vector
	mat4 translateMatrix = translate(mat4(1.0f), difference);
	MeteorModel = translateMatrix * MeteorModel;
}


void spawnMeteor()
{
	// Reset meteor model matrix
	MeteorModel = mat4(1.0);		
	isMeteorActive = true;

	// Translate meteor model matrix and position to camera position
	mat4 translateMatrix = translate(mat4(1.0f), cameraPosition);
	MeteorModel = translateMatrix * MeteorModel;
	meteorPosition = translateMatrix * meteorPosition;
}


void transformSun()
{
	// Rotate sun 
	SunModel = rotate(
		SunModel,
		radians(sunOrbitSpeed),
		vec3(0, 1, 0));
}


void transformPlanet()
{
	// Orbit planet around origin
	PlanetModel = translate(PlanetModel, vec3(-25.0f, 0.0f, 0.0f));
	PlanetModel = rotate(
		PlanetModel,
		radians(planetMoveSpeed),
		vec3(0, 1, 0));
	PlanetModel = translate(PlanetModel, vec3(25.0f, 0.0f, 0.0f));

	// Update planet position
	planetPosition = rotate(
		planetPosition,
		radians(planetMoveSpeed),
		vec3(0, 1, 0));
}


void transformNeptune() 
{
	// Orbit planet around origin
	NeptuneModel = translate(NeptuneModel, vec3(30.0f, -6.0f, 0.0f));
	NeptuneModel = rotate(
		NeptuneModel,
		radians(neptuneMoveSpeed),
		vec3(0, 1, 0));
	NeptuneModel = translate(NeptuneModel, vec3(-30.0f, 6.0f, 0.0f));

	// Update planet position
	neptunePosition = rotate(
		neptunePosition,
		radians(neptuneMoveSpeed),
		vec3(0, 1, 0));
}


void transformEarth()
{
	// Orbit planet around origin
	EarthModel = translate(EarthModel, vec3(40.0f, 8.0f, 0.0f));
	EarthModel = rotate(
		EarthModel,
		radians(earthMoveSpeed),
		vec3(0, 1, 0));
	EarthModel = translate(EarthModel, vec3(-40.0f, -8.0f, 0.0f));

	// Update planet position
	earthPosition = rotate(
		earthPosition,
		radians(earthMoveSpeed),
		vec3(0, 1, 0));
}


void transformMars()
{
	// Orbit planet around origin
	MarsModel = translate(MarsModel, vec3(-65.0f, -4.0f, 0.0f));
	MarsModel = rotate(
		MarsModel,
		radians(marsMoveSpeed),
		vec3(0, 1, 0));
	MarsModel = translate(MarsModel, vec3(65.0f, 4.0f, 0.0f));

	// Update planet position
	marsPosition = rotate(
		marsPosition,
		radians(marsMoveSpeed),
		vec3(0, 1, 0));
}


void computeMatrices() {

	transformSun();
	transformPlanet();
	transformNeptune();
	transformEarth();
	transformMars();

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!isMeteorActive)
			spawnMeteor();
	}

	if (isMeteorActive) {
		transformMeteor();
		checkForCollisions();
	}

	// Orbit camera around X axis
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition = rotateX(
			cameraPosition,
			radians(cameraOrbitSpeed));
	}

	// Orbit camera around X axis
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		cameraPosition = rotateX(
			cameraPosition,
			radians(-cameraOrbitSpeed));
	}

	// Orbit camera around Y axis
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition = rotateY(
			cameraPosition,
			radians(cameraOrbitSpeed));
	}

	// Orbit camera around Y axis
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition = rotateY(
			cameraPosition,
			radians(-cameraOrbitSpeed));
	}

	// Zoom in
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		normalizedVector = normalize(sunPosition - cameraPosition);
		cameraPosition += cameraMoveSpeed * normalizedVector;
	}

	// Zoom out
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		normalizedVector = normalize(sunPosition - cameraPosition);
		cameraPosition -= cameraMoveSpeed * normalizedVector;
	}

	// Decrease planet movement speed
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		planetMoveSpeed -= planetMoveSpeed * 0.05f;
		neptuneMoveSpeed -= neptuneMoveSpeed * 0.05f;
		earthMoveSpeed -= earthMoveSpeed * 0.05f;
		marsMoveSpeed -= marsMoveSpeed * 0.05f;
		sunOrbitSpeed -= sunOrbitSpeed * 0.05f;
	}

	// Increase planet movement speed
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		planetMoveSpeed += planetMoveSpeed * 0.05f;
		neptuneMoveSpeed += neptuneMoveSpeed * 0.05f;
		earthMoveSpeed += earthMoveSpeed * 0.05f;
		marsMoveSpeed += marsMoveSpeed * 0.05f;
		sunOrbitSpeed += sunOrbitSpeed * 0.05f;
	}

	// Respawn planets
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		collidedWithPlanet = false;
		collidedWithNeptune = false;
		collidedWithEarth = false;
		collidedWithMars = false;
	}

	// Reset camera position
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		cameraPosition = vec3(0.5f, 24.0f, -155.0f);
	}

	if (DEBUG_CAMERA)
		printf("%f %f %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

	if (DEBUG_METEOR)
		printf("%f %f %f\n", meteorPosition.x, meteorPosition.y, meteorPosition.z);

	// Update camera matrix
	View = glm::lookAt(
		cameraPosition,           // Camera is here
		sunPosition, // and looks here
		cameraUp                  // Head is up
	);

}
