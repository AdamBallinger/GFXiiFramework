#pragma once
#include "Vector3.h"
#include <vector>

class Triangle
{
public:
	Vertex		m_vertices[3];
public:
	Triangle();
	Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3);
	~Triangle();
	
	void SetVertices(Vector3 v0, Vector3 v1, Vector3 v2);
	void SetNormals(Vector3 n0, Vector3 n1, Vector3 n2);
	void SetTexCoords(Vector3 t0, Vector3 t1, Vector3 t2);
};

