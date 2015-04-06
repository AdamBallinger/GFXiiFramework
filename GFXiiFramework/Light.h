#ifndef light_h
#define light_h

#include "glm/glm.hpp"

class Light
{
private:

	glm::vec3 *lightDir;
	glm::vec3 *lightColor;

	float lightAmbient;
	float lightDiffuse;
	float lightSpecular;

public:

	Light();
	Light(glm::vec3);
	~Light();

	inline glm::vec3& GetLightDirection()
	{
		return *lightDir;
	}

	inline glm::vec3& GetLightColor()
	{
		return *lightColor;
	}

	inline float& GetLightAmbient()
	{
		return lightAmbient;
	}

	inline float& GetLightDiffuse()
	{
		return lightDiffuse;
	}

	inline float& GetLightSpecular()
	{
		return lightSpecular;
	}

	void SetLightDirection(glm::vec3);
	void SetLightColor(glm::vec3);
	void SetLightAmbient(float);
	void SetLightDiffuse(float);
	void SetLightSpecular(float);

};

#endif