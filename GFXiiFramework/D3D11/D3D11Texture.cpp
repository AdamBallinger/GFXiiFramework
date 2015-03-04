#if WIN32
#include <windows.h>
#endif

#include "../ImageIO.h"
#include "D3D11Texture.h"

D3D11Texture::D3D11Texture()
{
	m_textureRV = NULL;
}

D3D11Texture::~D3D11Texture()
{
	FreeTexture();
}

void D3D11Texture::CreateTextureFromFile(ID3D11Device* pDevice, const char* filename)
{
	//N.B. Limitation here, the default implementation here only loads texture in R8G8B8A8 format 
	//make sure your image file also contains an alpha channel.
	if(m_textureRV != NULL)
	{
		return;
	}

	unsigned char *buffer = NULL;
	int width;
	int height;
	int bpp;
	int nChannels;
	HRESULT hr;

	if(ImageIO::LoadTGA(filename, &buffer, &width, &height, &bpp, &nChannels) != E_IMAGEIO_SUCCESS)
	{
		return;
	}
	
	if(buffer == NULL)//this shouldn't happen, but let's do it anyway.
	{
		return;
	}

	D3D11_SUBRESOURCE_DATA initdata;
	ZeroMemory( &initdata, sizeof(initdata) );

	initdata.SysMemPitch = sizeof(unsigned char)*nChannels*width;
	initdata.pSysMem = buffer;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;

	hr = pDevice->CreateTexture2D( &desc, &initdata, &pTex );

	if ( SUCCEEDED( hr ) && pTex )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory( &srvd, sizeof(srvd) );

		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;

		hr = pDevice->CreateShaderResourceView( pTex, &srvd, &m_textureRV );

		if ( FAILED( hr ) )
		{
			pTex->Release();
		}

		if ( m_textureRV == NULL )
		{
			pTex->Release();
		}
	}

	free(buffer);
}

void D3D11Texture::CreateTextureFromData(void* data, int type, int width, int height, int bits)
{

}

void D3D11Texture::CreateTextureAsRenderTarget(int width, int height)
{
	
}

void D3D11Texture::CreateTextureAsDepthTarget(int width, int height)
{
	
}


void D3D11Texture::FreeTexture()
{
	if(m_textureRV != NULL)
	{
		m_textureRV->Release();
	}
}
