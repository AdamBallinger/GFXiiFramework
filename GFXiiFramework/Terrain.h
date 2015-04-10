#ifndef terrain_h
#define terrain_h

#include "OpenGL\OGLTexture.h"
#include "OpenGL\OGLMesh.h"

#include <string>

class Terrain
{
private:

	OGLMesh* mesh;

	OGLTexture* diffuseTexture;
	OGLTexture* specularTexture;
	OGLTexture* normalTexture;

public:

	Terrain();
	~Terrain();

	void SetMesh(LPCWSTR);
	void SetDiffuseTex(std::string);
	void SetSpecularTex(std::string);
	void SetNormalTex(std::string);
	void Render();

};

#endif