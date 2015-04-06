#ifndef terrain_h
#define terrain_h

#include "OpenGL\OGLTexture.h"

class Terrain
{
private:

	OGLTexture* m_heightmap;

public:

	Terrain();
	~Terrain();

	void CreateTerrainFromHeightMap(const char*);
	void Render();

};

#endif