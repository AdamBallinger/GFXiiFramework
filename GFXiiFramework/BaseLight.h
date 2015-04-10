#ifndef baselight_h
#define baselight_h

#include "glm\glm.hpp"

class BaseLight
{
private:

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

	float intensity;

public:

	BaseLight()
	{
		//Default values
		SetDirection(glm::vec3(0.0f, 0.0f, 0.0f));
		SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		SetIntensity(1.0f);
	}

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

	glm::vec3 GetPosition()
	{
		return position;
	}

	glm::vec3 GetDirection()
	{
		return direction;
	}

	glm::vec3 GetColor()
	{
		return color;
	}

	float GetIntensity()
	{
		return intensity;
	}
};

#endif