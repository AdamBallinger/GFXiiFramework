#include "SpotLight.h"

SpotLight::SpotLight()
{
	SetDirection(glm::vec3(0.0f));
	SetCutOff(35.0f);
}

void SpotLight::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void SpotLight::SetColor(glm::vec3 _color)
{
	color = _color;
}

void SpotLight::SetDirection(glm::vec3 _direction)
{
	direction = _direction;
}

void SpotLight::SetIntensity(float _intensity)
{
	intensity = _intensity;
}

void SpotLight::SetExponent(float _exponent)
{
	exponent = _exponent;
}

void SpotLight::SetCutOff(float _cutOff)
{
	cutOff = _cutOff;
}

glm::vec3 SpotLight::GetPosition()
{
	return position;
}

glm::vec3 SpotLight::GetColor()
{
	return color;
}

glm::vec3 SpotLight::GetDirection()
{
	return direction;
}

float SpotLight::GetIntensity()
{
	return intensity;
}

float SpotLight::GetExponent()
{
	return exponent;
}

float SpotLight::GetCutOff()
{
	return cutOff;
}