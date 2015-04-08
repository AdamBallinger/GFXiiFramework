#ifndef arealight_h
#define arealight_h

#include <glm\glm.hpp>

class AreaLight
{
private:

	glm::vec3 position;
	glm::vec3 color;
	
	float intensity;
	float constAtten;
	float linearAtten;
	float expAtten;

public:

	AreaLight();

	void SetPosition(glm::vec3);
	void SetColor(glm::vec3);
	void SetIntensity(float);
	void SetConstAtten(float);
	void SetLinearAtten(float);
	void SetExpAtten(float);

	glm::vec3 GetPosition();
	glm::vec3 GetColor();
	
	float GetIntensity();
	float GetConstAtten();
	float GetLinearAtten();
	float GetExpAtten();

};

#endif