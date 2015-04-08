#include "Light.h"

Light::Light()
{

}

Light::Light(glm::vec3 _color, float _intensity)
{
	color = _color;
	intensity = _intensity;
}

void Light::SetColor(glm::vec3 _color)
{
	color = _color;
}

void Light::SetIntensity(float _intensity)
{
	intensity = _intensity;
}