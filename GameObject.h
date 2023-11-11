#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "glm/glm.hpp"
#include "string"
#include <model.h>
class GameObject {
public:

	GameObject(float x, float y, float z, std::string soundPath);
	GameObject(glm::vec3 position, std::string soundPath, Model* text);
	//GameObject(Model* model, Shader* shader);
	glm::vec3 getObjectPosition() const;
	Model* gettextPath() const;
	std::string getSoundPath() const;
	const char * getSoundPathC() const;
	void moveObjectPosition(float escalar);
	void moveObjectPosition(float x, float y, float z);
	std::string toString() const;
	//Model *getModel() const;
	//Shader* getShader() const;
	//void renderObject(glm::mat4 view, glm::mat4  projection);

private:
	glm::vec3 objectPosition;
	std::string soundEffectPath;
	Model* textPath;
	/*glm::vec3 objectScale;
	glm::vec3 objectRotation;
	Model *objectModel;
	Shader* objectShader;*/
};

#endif // !1


