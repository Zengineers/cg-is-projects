#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getModelMatrix();
void setModelCenter(float center);
int isCapsLockOn();
int isCapitalKeyPressed(int KEY_CONSTANT);

#endif