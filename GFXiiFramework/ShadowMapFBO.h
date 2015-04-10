#ifndef shadowmapfbo_h
#define shadowmapfbo_h

#include "GLEW/include/glew.h"
#include <iostream>

class ShadowMapFBO
{
private:

	GLuint m_fbo;
	GLuint m_shadowmap;

public:

	ShadowMapFBO();
	~ShadowMapFBO();

	bool Init(unsigned int _windowWidth, unsigned int _windowHeight);

	void BindForWriting();
	void BindForReading(GLenum _textureUnit);

};

#endif