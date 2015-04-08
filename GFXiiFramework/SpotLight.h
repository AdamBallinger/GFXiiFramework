#ifndef spotlight_h
#define spotlight_h

#include <glm\glm.hpp>

class SpotLight
{
private:

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 direction;

	float intensity;
	float exponent; // Exponent Attenuation
	float cutOff; // Angle the lights cone.

public:

	SpotLight();

	void SetPosition(glm::vec3);
	void SetColor(glm::vec3);
	void SetDirection(glm::vec3);
	void SetIntensity(float);
	void SetExponent(float);
	void SetCutOff(float);
	
	glm::vec3 GetPosition();
	glm::vec3 GetColor();
	glm::vec3 GetDirection();

	float GetIntensity();
	float GetExponent();
	float GetCutOff();

};

#endif