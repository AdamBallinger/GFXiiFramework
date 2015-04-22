#ifndef worldstructure_h
#define worldstructure_h

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderable.h"
#include "OpenGL\OGLMesh.h"

class WorldStructure
{
private:

	glm::mat4 transformationMatrix; // Objects model transform matrix (Translation, scale & rotation)

	Renderable* mesh;

public:

	WorldStructure();
	WorldStructure(LPCWSTR);
	~WorldStructure();

	inline void LoadTransformationIdentity()
	{
		transformationMatrix = glm::mat4(1.0f);
	}

	inline glm::mat4 GetTransformationMatrix()
	{
		return transformationMatrix;
	}

	inline void SetTransformationMatrix(glm::mat4& _transformationMatrix)
	{
		transformationMatrix = _transformationMatrix;
	}

	inline void Translate(glm::vec3 _translation)
	{
		transformationMatrix = glm::translate(transformationMatrix, _translation);
	}

	inline void Scale(glm::vec3 _scale)
	{
		transformationMatrix = glm::scale(transformationMatrix, _scale);
	}

	inline void Rotate(float _degrees, glm::vec3 _axis)
	{
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(_degrees), _axis);
	}

	inline glm::vec3 ExtractPosition()
	{
		return glm::vec3(transformationMatrix[3]);
	}

	inline void SetDiffuseTexture(std::string _diffusePath)
	{
		OGLTexture* diffuse = new OGLTexture();
		diffuse->CreateTextureFromFile(_diffusePath.c_str());
		mesh->SetTexture(diffuse);
	}

	inline void SetSpecularTexture(std::string _specularPath)
	{
		OGLTexture* specular = new OGLTexture();
		specular->CreateTextureFromFile(_specularPath.c_str());
		mesh->SetSpecTexture(specular);
	}

	inline void SetNormalTexture(std::string _normalPath)
	{
		OGLTexture* normal = new OGLTexture();
		normal->CreateTextureFromFile(_normalPath.c_str());
		mesh->SetNormalTexture(normal);
	}

	void Render();

};

#endif