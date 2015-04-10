#include "ShadowMapFBO.h"

#define NULL 0

ShadowMapFBO::ShadowMapFBO()
{

}

ShadowMapFBO::~ShadowMapFBO()
{

}

bool ShadowMapFBO::Init(unsigned int _windowWidth, unsigned int _windowHeight)
{
	glGenFramebuffers(1, &m_fbo);

	glGenTextures(1, &m_shadowmap);
	glBindTexture(GL_TEXTURE_2D, m_shadowmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _windowWidth, _windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowmap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stdout, "FB Error, status: 0x%x\n", status);
		return false;
	}
	else
	{
		fprintf(stdout, "Shadowmap FB completed.");
		return true;
	}
}

void ShadowMapFBO::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void ShadowMapFBO::BindForReading(GLenum _textureUnit)
{
	glActiveTexture(_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_shadowmap);
}