#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "../RenderWindow.h"
#pragma once

#include "D3D11Shader.h"
#include "D3D11Cube.h"
#include "D3D11Mesh.h"
#include "D3D11Texture.h"

//We will use some Direct X Maths utilities
using namespace DirectX;


__declspec(align(16)) class D3D11Window : public RenderWindow
{
	private:
		struct ConstantBuffer
		{
			XMMATRIX		m_worldMat;
			XMMATRIX		m_viewMat;
			XMMATRIX		m_projection;
		};

	private:
		ConstantBuffer				m_cbuffer;
		XMMATRIX					m_world;
		XMMATRIX					m_view;
		XMMATRIX					m_proj;
		float						m_euler[3];
		
		ID3D11Device				*m_dxDev;
		ID3D11DeviceContext			*m_dxContext;
		ID3D11RenderTargetView		*m_backbuffer;
		ID3D11Texture2D				*m_depthStencil;
		ID3D11DepthStencilView		*m_depthStencilView;
		ID3D11SamplerState			*m_texSamplerDefaultState;
		IDXGISwapChain				*m_swapChain;
		ID3D11Buffer				*m_constantBuffer;
		D3D11ShaderProgram			*m_shaderProgram;

		Renderable					*m_mesh;
		D3D11Texture				*m_texture;
						
protected:

		BOOL InitD3DDevice (HWND hWnd);
		BOOL DestroyD3DDevice();
		void InitD3D11State();

	public:
					D3D11Window();
					D3D11Window(HINSTANCE hInstance, int width, int height);
					~D3D11Window();

		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);
		void		Render();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		//We need to make sure the memory allocation is 16 byte aligned
		//since SSE intrinsics are used in Direct X math utilities.
		void*		operator new (size_t size)
		{
			return _aligned_malloc( size, 16 );
		}

		void		operator delete( void* p )
		{
			_aligned_free( p );
		}

		BOOL		MouseLBDown ( int x, int y );
		BOOL		MouseLBUp ( int x, int y );
		BOOL		MouseMove ( int x, int y );
		void HandleKeyDown(WPARAM);
		void HandleMouseScroll(int);
};
