#include "Skybox.h"
#include "ImageIO.h"
#include "GLEW/include/glew.h"

Skybox::Skybox()
{

}

Skybox::~Skybox()
{

}

void Skybox::Init()
{
	// Load the skybox texture files for each face.
	m_textures[0] = new OGLTexture();
	m_textures[0]->CreateTextureFromFile("../asset/texture/tiles_colour.tga");

	m_textures[1] = new OGLTexture();
	m_textures[1]->CreateTextureFromFile("../asset/texture/sky_textures/sky_bk.tga");

	m_textures[2] = new OGLTexture();
	m_textures[2]->CreateTextureFromFile("../asset/texture/sky_textures/sky_lt.tga");

	m_textures[3] = new OGLTexture();
	m_textures[3]->CreateTextureFromFile("../asset/texture/sky_textures/sky_rt.tga");

	m_textures[4] = new OGLTexture();
	m_textures[4]->CreateTextureFromFile("../asset/texture/sky_textures/sky_tp.tga");

	m_textures[5] = new OGLTexture();
	m_textures[5]->CreateTextureFromFile("../asset/texture/sky_textures/sky_bt.tga");
}

void Skybox::Render()
{
	
}