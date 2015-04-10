#pragma once

#include "OpenGL\OGLTexture.h"

//This is an abstract class
//It cannot be instantiated without being inherited by a concrete class
class Renderable
{
	protected:
		OGLTexture				*m_tex;
		OGLTexture				*m_spec;
		OGLTexture				*m_norm;

	public:

		//this is a pure virtual function
		Renderable() { m_tex = 0; m_norm = 0; m_spec = 0; }
		virtual void			Render() = 0;
		
		inline void				SetTexture(OGLTexture* tex)
		{
			m_tex = tex;
		}

		inline void				SetSpecTexture(OGLTexture* _spec)
		{
			m_spec = _spec;
		}

		inline void				SetNormalTexture(OGLTexture* _norm)
		{
			m_norm = _norm;
		}
};