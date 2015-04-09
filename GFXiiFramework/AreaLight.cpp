#include "AreaLight.h"

AreaLight::AreaLight() : AreaLight::BaseLight()
{
	SetConstAtten(0.0f);
	SetLinearAtten(0.0f);
	SetExpAtten(0.0f);
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