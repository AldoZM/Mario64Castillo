#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Light 
{
public:
	// Light Attributes
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec4 Color;
	glm::vec4 Power;
	int       alphaIndex;
	float     distance;

	Light(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4 power = glm::vec4(250.0f, 250.0f, 250.0f, 1.0f),
		int alphaIndex = 10, 
		float distance = 10.0f) 
	{
		this->Position = position;
		this->Direction = direction;
		this->Color = color;
		this->Power = power;
		this->alphaIndex = alphaIndex;
		this->distance = distance;
	}
	~Light() {}

private:

};

#endif
