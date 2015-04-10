#ifndef spotlight_h
#define spotlight_h

#include "glm\glm.hpp"
#include "BaseLight.h"

class SpotLight : public BaseLight
{
private:

	float exponent; // Exponent Attenuation
	float cutOff; // Angle the lights cone.

public:

	SpotLight();

	void SetExponent(float);
	void SetCutOff(float);

	float GetExponent();
	float GetCutOff();

};

#endif