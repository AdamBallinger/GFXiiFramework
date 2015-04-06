#ifndef skybox_h
#define skybox_h

#include "OpenGL/OGLCube.h"
#include "glm/glm.hpp"

class Skybox
{
private:

	// Store texture for the skybox (Cubemap)
	OGLTexture* m_texture;

	glm::vec3 m_position;

	float dimensions = 512.0f;

	struct SimpleVertex
	{
		float position[3];
		float colour[3];
		float uv[2];
	};

	unsigned int	m_vao;
	unsigned int	m_vbo_verts;
	unsigned int	m_vbo_colours;
	unsigned int	m_vbo_indices;

public:

	Skybox();
	~Skybox();

	void Init();

	void Render();

	inline void SetPosition(glm::vec3 _position)
	{
		m_position = _position;
	}

};

#endif