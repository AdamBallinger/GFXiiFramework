#ifndef directionallight_h
#define directionallight_h

#include "glm\glm.hpp"

class DirectionalLight
{
private:

	glm::vec3 direction;
	glm::vec3 color;

	float intensity;

public:

	DirectionalLight();

	void SetDirection(glm::vec3);
	void SetColor(glm::vec3);
	void SetIntensity(float);

	glm::vec3 GetDirection();
	glm::vec3 GetColor();
	
	float GetIntensity();

};

#endif