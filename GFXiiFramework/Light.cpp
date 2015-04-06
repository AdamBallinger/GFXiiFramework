#include "Light.h"

Light::Light()
{
	// Initialise default light properties.
	SetLightDirection(glm::vec3(0.0f, 0.0f, 0.0f));
	SetLightAmbient(0.1f);
	SetLightDiffuse(0.9f);
	SetLightSpecular(1.0f);
}

Light::Light(glm::vec3 _lightColor) : Light()
{
	SetLightColor(_lightColor);
}

Light::~Light()
{
	delete lightDir;
	delete lightColor;
}

void Light::SetLightDirection(glm::vec3 _lightDir)
{
	lightDir = &_lightDir;
}

void Light::SetLightColor(glm::vec3 _lightColor)
{
	lightColor = &_lightColor;
}

void Light::SetLightAmbient(float _ambient)
{
	lightAmbient = _ambient;
}

void Light::SetLightDiffuse(float _diffuse)
{
	lightDiffuse = _diffuse;
}

void Light::SetLightSpecular(float _specular)
{
	lightSpecular = _specular;
}