#pragma once

#include <Windows.h>
#include "../Renderable.h"
#include "../Triangle.h"


class OGLMesh : public Renderable
{
private:
	unsigned int	m_vao;
	unsigned int	m_vbo_verts;
	int							m_numtriangles;

public:
	OGLMesh();
	OGLMesh(LPCWSTR filename);
	~OGLMesh();

	void LoadAndBuildMeshFromOBJFile(LPCWSTR filename);

	void Render();

};