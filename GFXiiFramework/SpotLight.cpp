#include "SpotLight.h"

SpotLight::SpotLight() : SpotLight::BaseLight()
{
	SetExponent(0.0f);
	SetCutOff(35.0f);
}

void SpotLight::SetExponent(float _exponent)
{
	exponent = _exponent;
}

void SpotLight::SetCutOff(float _cutOff)
{
	cutOff = _cutOff;
}

float SpotLight::GetExponent()
{
	return exponent;
}

float SpotLight::GetCutOff()
{
	return cutOff;
}