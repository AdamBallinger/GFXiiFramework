#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	//Default values
	SetDirection(glm::vec3(0.0f, 0.0f, 0.0f));
	SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	SetIntensity(1.0f);
}

void DirectionalLight::SetDirection(glm::vec3 _direction)
{
	direction = _direction;
}

void DirectionalLight::SetColor(glm::vec3 _color)
{
	color = _color;
}

void DirectionalLight::SetIntensity(float _intensity)
{
	intensity = _intensity;
}

glm::vec3 DirectionalLight::GetDirection()
{
	return direction;
}

glm::vec3 DirectionalLight::GetColor()
{
	return color;
}

float DirectionalLight::GetIntensity()
{
	return intensity;
}