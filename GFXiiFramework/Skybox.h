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

	// Override OGLCube
	virtual void Render();

	inline void SetPosition(glm::vec3 _position)
	{
		m_position = _position;
	}

};

#endif