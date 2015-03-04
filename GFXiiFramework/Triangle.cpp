#include "Triangle.h"

Triangle::Triangle()
{
	
}

Triangle::Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3)
{
	SetVertices(pos1, pos2, pos3);
}


Triangle::~Triangle()
{
}

void Triangle::SetVertices(Vector3 v0, Vector3 v1, Vector3 v2)
{
	m_vertices[0].m_position = v0;
	m_vertices[1].m_position = v1;
	m_vertices[2].m_position = v2;
}

void Triangle::SetNormals(Vector3 n0, Vector3 n1, Vector3 n2)
{
	m_vertices[0].m_normal = n0;
	m_vertices[1].m_normal = n1;
	m_vertices[2].m_normal = n2;
}

void Triangle::SetTexCoords(Vector3 t0, Vector3 t1, Vector3 t2)
{
	m_vertices[0].m_texcoords = t0;
	m_vertices[1].m_texcoords = t1;
	m_vertices[2].m_texcoords = t2;
}