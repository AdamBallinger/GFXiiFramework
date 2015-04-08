#include "AreaLight.h"

AreaLight::AreaLight()
{
	SetPosition(glm::vec3(0.0f));
	SetColor(glm::vec3(1.0f));
	SetIntensity(1.0f);
	SetConstAtten(0.0f);
	SetLinearAtten(0.0f);
	SetExpAtten(0.0f);
}

void AreaLight::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void AreaLight::SetColor(glm::vec3 _color)
{
	color = _color;
}

void AreaLight::SetIntensity(float _intensity)
{
	intensity = _intensity;
}

void AreaLight::SetConstAtten(float _constAtten)
{
	constAtten = _constAtten;
}

void AreaLight::SetLinearAtten(float _linearAtten)
{
	linearAtten = _linearAtten;
}

void AreaLight::SetExpAtten(float _expAtten)
{
	expAtten = _expAtten;
}

glm::vec3 AreaLight::GetPosition()
{
	return position;
}

glm::vec3 AreaLight::GetColor()
{
	return color;
}

float AreaLight::GetIntensity()
{
	return intensity;
}

float AreaLight::GetConstAtten()
{
	return constAtten;
}

float AreaLight::GetLinearAtten()
{
	return linearAtten;
}

float AreaLight::GetExpAtten()
{
	return expAtten;
}