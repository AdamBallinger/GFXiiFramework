#pragma once

#include <vector>
#include "../ShaderBase.h"

class D3D11ShaderProgram
{
	private:
		ID3D11VertexShader*			m_vertexShader;
		ID3D11PixelShader*			m_fragmentShader;
		ID3DBlob*					m_vsBlob;
		ID3DBlob*					m_fsBlob;
		ID3D11DeviceContext*		m_context;
		ID3D11Device*				m_device;
		int							m_shaderCount;

	public:
									D3D11ShaderProgram()
									{
										m_device = NULL;
										m_context = NULL;
										m_vertexShader = NULL;
										m_fragmentShader = NULL;
										m_vsBlob = NULL;
										m_fsBlob = NULL;
									}

									D3D11ShaderProgram( ID3D11Device* dev, ID3D11DeviceContext* cntx )
									{
										m_device = dev;
										m_context = cntx;
										m_vertexShader = NULL;
										m_fragmentShader = NULL;
										m_vsBlob = NULL;
										m_fsBlob = NULL;
									}

		virtual						~D3D11ShaderProgram();
		
		inline void					SetContext( ID3D11DeviceContext* cntx )
		{
			m_context = cntx;
		}

		inline void					SetDevice( ID3D11Device* dev )
		{
			m_device = dev;
		}

		inline ID3DBlob*			GetVSBlob ()
		{
			return m_vsBlob;
		}

		inline ID3DBlob*			GetFSBlob ()
		{
			return m_fsBlob;
		}

		inline ID3D11Device*		GetDevice()
		{
			return m_device;
		}

		inline ID3D11DeviceContext*	GetContext()
		{
			return m_context;
		}

		EShaderError				AttachAndCompileShaderFromFile(LPCWSTR filename, EShaderType type);
		void						DeleteShaderProgram();
		void						ActivateShaderProgram();
		void						DeactivateShaderProgram();
};
