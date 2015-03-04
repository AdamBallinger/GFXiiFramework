#include "D3D11Window.h"

D3D11Window::D3D11Window()
{
	m_shaderProgram = NULL;

	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

D3D11Window::D3D11Window( HINSTANCE hInstance, int width, int height )
{
	//InitWindow(hInstance, width, height);	
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

D3D11Window::~D3D11Window()
{
	if ( m_shaderProgram ) delete m_shaderProgram;

	delete m_mesh;
	delete m_texture;

	DestroyD3DDevice();
}

BOOL D3D11Window::InitD3DDevice ( HWND hWnd )
{
	DXGI_SWAP_CHAIN_DESC dxscd;

	ZeroMemory( &dxscd, sizeof(DXGI_SWAP_CHAIN_DESC) );

	dxscd.BufferCount = 1;
	dxscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxscd.OutputWindow = hWnd;
	dxscd.SampleDesc.Count = 1;
	dxscd.Windowed = TRUE;
	
	HRESULT result = D3D11CreateDeviceAndSwapChain( NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &dxscd, &m_swapChain, &m_dxDev, NULL, &m_dxContext );

	if ( result != S_OK )
		return FALSE;

	//Create a back buffer for double buffering
	ID3D11Texture2D *pBackBufferSurface;
	m_swapChain->GetBuffer( 0, __uuidof(pBackBufferSurface), (void**)&pBackBufferSurface );

	m_dxDev->CreateRenderTargetView( pBackBufferSurface, NULL, &m_backbuffer );
	pBackBufferSurface->Release();

	// Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = m_width;
    descDepth.Height = m_height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    
	result = m_dxDev->CreateTexture2D( &descDepth, nullptr, &m_depthStencil );
    if( FAILED( result ) )
        return FALSE;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    result = m_dxDev->CreateDepthStencilView( m_depthStencil, &descDSV, &m_depthStencilView );

	m_dxContext->OMSetRenderTargets( 1, &m_backbuffer, m_depthStencilView );

	//Init shader
	m_shaderProgram = new D3D11ShaderProgram(m_dxDev, m_dxContext);
	m_shaderProgram->AttachAndCompileShaderFromFile( L"../asset/shader/hlsl/basic_vs.hlsl", SHADER_VERTEX );
	m_shaderProgram->AttachAndCompileShaderFromFile( L"../asset/shader/hlsl/basic_fs.hlsl", SHADER_FRAGMENT );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( ConstantBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	m_dxDev->CreateBuffer( &bd, NULL, &m_constantBuffer );

	m_mesh = new D3D11Mesh(L"../asset/models/house.obj", m_shaderProgram);
	
	//Create a texture sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_dxDev->CreateSamplerState( &sampDesc, &m_texSamplerDefaultState );
	
	m_texture = new D3D11Texture();
	m_texture->CreateTextureFromFile( m_dxDev, "../asset/texture/house_diffuse.tga" );

	m_mesh->SetTexture( m_texture );
	
	
	return TRUE;
}

BOOL D3D11Window::DestroyD3DDevice ()
{
	m_swapChain->Release();
	m_backbuffer->Release();
	m_depthStencil->Release();
	m_depthStencilView->Release();
	m_constantBuffer->Release();
	m_texSamplerDefaultState->Release();
	m_dxContext->Release();
	m_dxDev->Release();
	
	return TRUE;
}

BOOL D3D11Window::InitWindow( HINSTANCE hInstance, int width, int height )
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"D3D11Window", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;
		
	m_width = width;
	m_height = height;

	InitD3DDevice( m_hwnd );

	return TRUE;
}

void D3D11Window::Render()
{
	Renderable* pRenderable = static_cast<Renderable*>(m_mesh);

	float clearcolour[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	float inc = -XM_PIDIV4*0.0005;

	m_euler[0] = -XM_PIDIV4*0.4;
	m_euler[1] = m_euler[1] > XM_2PI ? 0 : m_euler[1] + inc;
	m_euler[2] = 0.0;
	
	//Remember, D3D11 uses a LEFT HANDED system
	//Setting up view matrix
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(eye, at, up);
	
	m_dxContext->ClearRenderTargetView( m_backbuffer, clearcolour );
	
	m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0.0 );

	//Setting world matrix 
	m_world = XMMatrixIdentity();
	//add some idle rotations using euler angles;
	m_world = XMMatrixRotationY( m_euler[2] )*XMMatrixRotationY( m_euler[1] )*XMMatrixRotationX( m_euler[0] );
	
	//copy the matrices to constant buffer for shader use
	m_cbuffer.m_worldMat = m_world;
	m_cbuffer.m_viewMat = m_view;
	m_cbuffer.m_projection = m_proj;

	m_dxContext->UpdateSubresource( m_constantBuffer, 0, NULL, &m_cbuffer, 0, 0 );

	m_shaderProgram->ActivateShaderProgram();
	m_dxContext->VSSetConstantBuffers( 0, 1, &m_constantBuffer );
	
	m_dxContext->PSSetSamplers( 0, 1, &m_texSamplerDefaultState );
	
	m_mesh->Render();

	m_swapChain->Present(0, 0);
}

void D3D11Window::Resize( int width, int height )
{
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	
	float aspectratio = (float)width / (float)height;
	m_dxContext->RSSetViewports( 1, &viewport );

	m_proj = XMMatrixPerspectiveFovLH ( XM_PIDIV2*0.66667, aspectratio, 0.1f, 1000.0f );
}

void D3D11Window::DestroyRenderWindow()
{
	DestroyWindow(m_hwnd);
}

BOOL D3D11Window::MouseLBDown ( int x, int y )
{
	return TRUE;
}

BOOL D3D11Window::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL D3D11Window::MouseMove ( int x, int y )
{
	return TRUE;
}