#pragma once

#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_inverse.hpp"

#include "../RenderWindow.h"
#include "OGLCube.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"

#include "../Skybox.h"
#include "../Camera.h"
#include "../Terrain.h"
#include "../DirectionalLight.h"
#include "../AreaLight.h"
#include "../SpotLight.h"
#include "../ShadowMapFBO.h"

#include "../WorldStructure.h"
#include "../Player.h"
#include "../RacingStructure.h"

class OGLWindow : public RenderWindow
{
	private:
		float	    m_euler[3];
		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		int			m_width;
		int			m_height;

		float linearAtten = 0.05f;

		WorldStructure* house;
		WorldStructure* house_med;
		WorldStructure* super_secret;
		WorldStructure* plane;

		RacingStructure* raceRing;
		std::vector<glm::mat4> racePointTransforms;

		ShadowMapFBO *shadowmapFBO;

		Player* player;

		Camera* camera;
		Skybox* skybox;
		Terrain* terrain;

		//Lights
		DirectionalLight* directionalLight;
		AreaLight* areaLight;
		SpotLight* spotLight;

		// Matrices
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 normal;
		glm::mat4 transformation;

		//Declear an OGL shader program
		OGLShaderProgram		*m_shader;
		OGLShaderProgram		*m_skybox_shader;
		int						m_texDefaultSampler;

		// Store the byte values for required key inputs.
		static const byte W = 0x57;
		static const byte A = 0x41;
		static const byte S = 0x53;
		static const byte D = 0x44;
		static const byte Z = 0x5A;
		static const byte X = 0x58;
		static const byte Q = 0x51;
		static const byte E = 0x45;

protected:

		HGLRC CreateOGLContext (HDC hdc);
		BOOL DestroyOGLContext();
		void InitOGLState();

	public:
					OGLWindow();
					OGLWindow(HINSTANCE hInstance, int width, int height);
					~OGLWindow();
		
		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);

		void		Render();
		void		ShadowMapPass();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		BOOL		MouseLBDown ( int x, int y );
		BOOL		MouseLBUp ( int x, int y );
		BOOL		MouseMove ( int x, int y );
		void HandleKeyDown();
		void HandleMouseScroll(int);
		void SetUniforms();
		void BuildMatrices();
		void SetVSync(bool);

		void CheckCollisions();
		void CreateRacingRings();
		void ResetRacingRings();
};
