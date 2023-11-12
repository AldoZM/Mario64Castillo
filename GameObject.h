#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "glm/glm.hpp"
#include "string"

class GameObject {
public:

	GameObject(float x, float y, float z, std::string soundPath, glm::vec3 angles);
	GameObject(glm::vec3 position, std::string soundPath);
	//GameObject(Model* model, Shader* shader);
	glm::vec3 getObjectPosition() const;
	std::string getSoundPath() const;
	const char * getSoundPathC() const;
	void moveObjectPosition(float escalar);
	void moveObjectPosition(float x, float y, float z);
<<<<<<< HEAD
	void rotateObjectZ(float angleToAdd);
	float getAngleZ() const;
=======
	void rotateObject(float angleToAdd);
>>>>>>> 3c6362538cdd39e3d144ade95fe1c31b22c15c69
	std::string toString() const;
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


