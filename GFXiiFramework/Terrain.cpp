#include "Terrain.h"

Terrain::Terrain()
{

}

Terrain::~Terrain()
{
	
}

void Terrain::CreateTerrainFromHeightMap(const char* _filepath)
{
	m_heightmap = new OGLTexture();
	m_heightmap->CreateTextureFromFile(_filepath);
}

void Terrain::Render()
{

}