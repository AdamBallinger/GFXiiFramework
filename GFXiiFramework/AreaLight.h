#ifndef arealight_h
#define arealight_h

#include "glm\glm.hpp"
#include "BaseLight.h"

class AreaLight : public BaseLight
{
private:
	
	float constAtten;
	float linearAtten;
	float expAtten;

public:

	AreaLight();

	void SetConstAtten(float);
	void SetLinearAtten(float);
	void SetExpAtten(float);
	
	float GetConstAtten();
	float GetLinearAtten();
	float GetExpAtten();

};

#endif