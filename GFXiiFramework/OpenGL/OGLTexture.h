#ifndef __TEXTUREGL_H__
#define __TEXTUREGL_H__

#include "BaseTexture.h"

class OGLTexture : public BaseTexture
{
	private:

	public:
		unsigned int					m_syshandle;
		unsigned int					m_apphandle;

	public:
										OGLTexture();
		virtual							~OGLTexture();

		virtual void					CreateTextureFromFile(const char* filename);
		virtual void					CreateTextureFromData(void* data, int type, int width, int height, int bits);

		virtual void					CreateTextureAsRenderTarget(int width, int height);

		virtual void					CreateTextureAsDepthTarget(int width, int height);
		virtual void					FreeTexture();
};

#endif
