#include <stdio.h>
#include <vector>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include "D3D11Shader.h"

D3D11ShaderProgram::~D3D11ShaderProgram()
{
	DeleteShaderProgram();
}
		
EShaderError D3D11ShaderProgram::AttachAndCompileShaderFromFile(LPCWSTR filename, EShaderType type)
{
	ID3DBlob* pErrorBlob = NULL;
	HRESULT hr = S_OK;
	
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		
	if ( type == SHADER_VERTEX )
	{
		hr = D3DCompileFromFile( filename, NULL, NULL, "main", "vs_4_0",
			shaderFlags, 0, &m_vsBlob, &pErrorBlob);
		
	}
	else if ( type == SHADER_FRAGMENT )
	{
		hr = D3DCompileFromFile( filename, NULL, NULL, "main", "ps_4_0",
			shaderFlags, 0, &m_fsBlob, &pErrorBlob);
	}

	if ( FAILED(hr) )
	{
		if ( pErrorBlob )
		{
			fprintf(stdout, "Shader output: %s\n",
				reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()) );

			pErrorBlob->Release();
		}

		return SHADER_ERROR_COMPILE;
	}

	if ( pErrorBlob ) pErrorBlob->Release();

	if ( type == SHADER_VERTEX )
	{
		hr = m_device->CreateVertexShader( m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), NULL, &m_vertexShader );
	}
	else if ( type == SHADER_FRAGMENT )
	{
		hr = m_device->CreatePixelShader( m_fsBlob->GetBufferPointer(), m_fsBlob->GetBufferSize(), NULL, &m_fragmentShader );
	}
	
	if ( FAILED(hr) )
	{
		return SHADER_ERROR_CREATE;
	}

	return SHADER_ERROR_NONE;
}
		
void D3D11ShaderProgram::DeleteShaderProgram()
{
	if ( m_vertexShader ) m_vertexShader->Release();

	if ( m_fragmentShader ) m_fragmentShader->Release();

	if ( m_vsBlob ) m_vsBlob->Release();

	if ( m_fsBlob ) m_fsBlob->Release();
}
		
void D3D11ShaderProgram::ActivateShaderProgram()
{
	m_context->VSSetShader( m_vertexShader, NULL, 0 );
	m_context->PSSetShader( m_fragmentShader, NULL, 0 );
}
		
void D3D11ShaderProgram::DeactivateShaderProgram()
{

}