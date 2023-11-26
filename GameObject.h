#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "glm/glm.hpp"
#include "string"

class GameObject {
public:

	GameObject(float x, float y, float z, 
			std::string soundPath="", 
			glm::vec3 angles = glm::vec3(0.0f, -90.0f, 0.0f));
	GameObject(glm::vec3 position, std::string soundPath="");
	//GameObject(Model* model, Shader* shader);
	glm::vec3 getObjectPosition() const;
	std::string getSoundPath() const;
	const char * getSoundPathC() const;
	void moveObjectPosition(float escalar);
	void moveObjectPosition(float x, float y, float z);
	void rotateObjectZ(float angleToAdd);
	float getAngleZ() const;
	std::string toString() const;
	void setObjectPosition(glm::vec3 newPosition);
	//Model *getModel() const;
	//Shader* getShader() const;
	//void renderObject(glm::mat4 view, glm::mat4  projection);

private:
	glm::vec3 objectPosition;
	glm::vec3 objectAngles;
	std::string soundEffectPath;
	/*glm::vec3 objectScale;
	glm::vec3 objectRotation;
	Model *objectModel;
	Shader* objectShader;*/
};

#endif // !1


