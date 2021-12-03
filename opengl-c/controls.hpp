#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void setInitialMatrices();
void computeMatrices();
void checkForCollisions();
void transformMeteor();
void spawnMeteor();
void transformSun();
void transformPlanet();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getSunModelMatrix();
glm::mat4 getPlanetModelMatrix();
glm::mat4 getMeteorModelMatrix();
glm::mat4 getNeptuneModelMatrix();
glm::mat4 getEarthModelMatrix();
glm::mat4 getMarsModelMatrix();
int isCapsLockOn();
int isCapitalKeyPressed(int KEY_CONSTANT);
bool meteorActive();
bool meteorCollidedWithPlanet();
bool meteorCollidedWithNeptune();
bool meteorCollidedWithEarth();
bool meteorCollidedWithMars();

#endif