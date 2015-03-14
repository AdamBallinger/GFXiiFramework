#ifndef skybox_h
#define skybox_h

#include "OpenGL/OGLCube.h"
#include "glm/glm.hpp"

class Skybox : public OGLCube
{
private:

	// Store textures for each face of the sky box 
	// Index: 0 -> Front
	// Index: 1 -> Back
	// Index: 2 -> Left
	// Index: 3 -> Right
	// Index: 4 -> Top
	// Index: 5 -> Bottom
	OGLTexture* m_textures[6];

public:

	Skybox();
	~Skybox();

	void Init();

	// Override OGLCube
	virtual void Render();

};

#endif