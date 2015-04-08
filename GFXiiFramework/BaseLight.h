#ifndef baselight_h
#define baselight_h

#include <glm\glm.hpp>

class BaseLight
{
private:

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

	float intensity;

public:

	BaseLight();

	void SetPosition(glm::vec3 _position)
	{
		position = _position;
	}

	void SetDirection(glm::vec3 _direction)
	{
		direction = _direction;
	}

	void SetColor(glm::vec3 _color)
	{
		color = _color;
	}

	void SetIntensity(float _intensity)
	{
		intensity = _intensity;
	}

};

#endif