#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>


//GameObject::GameObject(Model* model, Shader *shader) {
//	this->objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
//}

GameObject::GameObject(float x, float y, float z, 
					std::string soundPath = "", 
					glm::vec3 angles = glm::vec3(0.0f, -90.0f, 0.0f)) {
	this->objectPosition = glm::vec3(x, y, z);
	this->soundEffectPath = soundPath;
	this->objectAngles = angles;
}

GameObject::GameObject(glm::vec3 position, std::string soundPath = "") {
	this->objectPosition = position;
	this->soundEffectPath = soundPath;
	this->objectAngles = glm::vec3(0.0f, -90.0f, 0.0f);
}

glm::vec3 GameObject::getObjectPosition() const {
	return this->objectPosition;
}

void GameObject::moveObjectPosition(float escalar) {
	this->objectPosition += escalar;
}

void GameObject::moveObjectPosition(float x, float y, float z) {
	glm::vec3 vectorToAdd = glm::vec3(x, y, z);
	this->objectPosition += vectorToAdd;
}

<<<<<<< HEAD
void GameObject::rotateObjectZ(float angleToAdd) {
=======
void GameObject::rotateObject(float angleToAdd) {
>>>>>>> 3c6362538cdd39e3d144ade95fe1c31b22c15c69
	this->objectAngles.z += angleToAdd;
	if ((int)this->objectAngles.z > 360) {
		this->objectAngles.z = 0.0f;
	}
	else if ((int)this->objectAngles.z < 0) {
		this->objectAngles.z = 360.0f;
	}
}

<<<<<<< HEAD
float GameObject::getAngleZ() const {
	return this->objectAngles.z;
}

=======
>>>>>>> 3c6362538cdd39e3d144ade95fe1c31b22c15c69
std::string GameObject::toString() const {
	return "Posicion: " + glm::to_string(this->objectPosition) + " SoundPath: " + this->soundEffectPath;
}

std::string GameObject::getSoundPath() const {
	return this->soundEffectPath;
}

const char* GameObject::getSoundPathC() const {
	return this->soundEffectPath.c_str();
}


//Model* GameObject::getModel() const {
//	return this->objectModel;
//}
//
//Shader* GameObject::getShader() const {
//	return this->objectShader;
//}


//void GameObject::renderObject(glm::mat4 view, glm::mat4  projection) {
//	objectShader->use();
//
//	objectShader->setMat4("projection", projection);
//	objectShader->setMat4("view", view);
//
//	glm::mat4 model = glm::mat4(1.0f);
//	model = glm::translate(model, objectPosition); 
//	//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
//	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
//}