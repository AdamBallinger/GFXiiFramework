#ifndef light_h
#define light_h

#include "glm/glm.hpp"

class Light
{
private:

	glm::vec3 color;
	float intensity;

public:

	Light();
	Light(glm::vec3 _color, float _intensity);

	glm::vec3 GetColor()
	{
		return color;
	}

	float GetIntensity()
	{
		return intensity;
	}

	void SetColor(glm::vec3);
	void SetIntensity(float);

};

#endif