#pragma once

#include <Windows.h>
#include "../Renderable.h"
#include "../Triangle.h"
#include "D3D11Shader.h"

class D3D11Mesh :	public Renderable
{
	private: 
		D3D11ShaderProgram*			m_shader;
		ID3D11InputLayout*			m_vertexLayout;
		ID3D11Buffer*				m_vertexBuffer;
		ID3D11Buffer*				m_indexBuffer;
		int							m_numtriangles;

	public:
		D3D11Mesh();
		D3D11Mesh(LPCWSTR filename, D3D11ShaderProgram* shader);
		~D3D11Mesh();

		void LoadAndBuildMeshFromOBJFile(LPCWSTR filename, D3D11ShaderProgram* shader);

		void Render();

};

