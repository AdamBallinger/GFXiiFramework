#ifndef __TEXTURED3D11_H__
#define __TEXTURED3D11_H__

#include <d3d11_1.h>

#include "../BaseTexture.h"

class D3D11Texture : public BaseTexture
{
	public:
		ID3D11ShaderResourceView*		m_textureRV;

	public:
										D3D11Texture();
		virtual							~D3D11Texture();

		virtual void					CreateTextureFromFile(ID3D11Device* pDevice, const char* filename);
		virtual void					CreateTextureFromData(void* data, int type, int width, int height, int bits);

		virtual void					CreateTextureAsRenderTarget(int width, int height);

		virtual void					CreateTextureAsDepthTarget(int width, int height);
		virtual void					FreeTexture();
};

#endif
