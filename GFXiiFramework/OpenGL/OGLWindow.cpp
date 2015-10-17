#include "OGLWindow.h"
#include "../Resource.h"
#include "../GLEW/include/glew.h"

#include <Windows.h>
#include <iostream>

OGLWindow::OGLWindow()
{
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

OGLWindow::~OGLWindow()
{
	delete house;
	delete house_med;
	delete super_secret;
	delete plane;

	delete player;

	delete raceRing;

	delete m_shader;
	delete m_skybox_shader;

	delete camera;
	delete skybox;
	delete terrain;
	delete shadowmapFBO;

	delete directionalLight;
	delete areaLight;
	delete spotLight;

	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers( 1, (GLuint*)(&m_texDefaultSampler) );

	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"Flight Simulator", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;

	glewInit();

	ShowCursor(false);

	InitOGLState();

	m_width = width;
	m_height = height;

	SetCursorPos(m_width / 2, m_height / 2);

	camera = new Camera();
	camera->InitCamera();
	camera->SetCameraPosition(&glm::vec3(0.0f, 820.0f, 10.0f));
	camera->SetCameraFOV(70);

	directionalLight = new DirectionalLight();
	directionalLight->SetDirection(glm::vec3(-1.0f, -0.2f, 0.0f));
	directionalLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	directionalLight->SetIntensity(1.0f);

	areaLight = new AreaLight();
	areaLight->SetPosition(glm::vec3(325.0f, 680.0f, -240.0f));
	areaLight->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
	areaLight->SetIntensity(1.0f);
	//areaLight->SetConstAtten(0.1f);
	areaLight->SetLinearAtten(0.06f);
	//areaLight->SetExpAtten(0.0f);

	spotLight = new SpotLight();
	spotLight->SetPosition(glm::vec3(10.0f, 800.0f, -20.0f));
	spotLight->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	spotLight->SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	spotLight->SetIntensity(1.0f);
	spotLight->SetExponent(0.08f);
	spotLight->SetCutOff(90.0);

	shadowmapFBO = new ShadowMapFBO();

	skybox = new Skybox();
	skybox->Init();

	terrain = new Terrain();
	terrain->SetMesh(L"../asset/models/terrain.obj");
	terrain->SetDiffuseTex("../asset/texture/terrain_diffuse.tga");
	terrain->SetSpecularTex("../asset/texture/terrain_spec.tga");
	terrain->SetNormalTex("../asset/texture/terrain_normal.tga");

	house = new WorldStructure(L"../asset/models/house.obj");
	house->SetDiffuseTexture("../asset/texture/house_diffuse.tga");
	house->SetSpecularTexture("../asset/texture/house_spec.tga");
	house->SetNormalTexture("../asset/texture/house_normal.tga");

	house->Translate(glm::vec3(400.0f, 650.0f, -350.0f));
	house->Scale(glm::vec3(20.0f, 20.0f, 20.0f));
	house->Rotate(45, glm::vec3(0.0f, 1.0f, 0.0f));

	house_med = new WorldStructure(L"../asset/models/house_med.obj");
	house_med->SetDiffuseTexture("../asset/texture/house_med_diffuse.tga");
	house_med->SetSpecularTexture("../asset/texture/house_med_spec.tga");
	house_med->SetNormalTexture("../asset/texture/house_med_normal.tga");

	house_med->Translate(glm::vec3(-350.0f, 610.0f, -880.0f));
	house_med->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	house_med->Rotate(-45, glm::vec3(0.0f, 1.0f, 0.0f));

	super_secret = new WorldStructure(L"../asset/models/secret.obj");
	super_secret->SetDiffuseTexture("../asset/texture/secret_diffuse.tga");

	super_secret->Translate(glm::vec3(30.0f, 600.0f, -80.0f));
	super_secret->Scale(glm::vec3(5.0f, 5.0f, 5.0f));
	super_secret->Rotate(-20.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	plane = new WorldStructure(L"../asset/models/plane.obj");
	plane->SetDiffuseTexture("../asset/texture/plane_diffuse.tga");
	plane->SetSpecularTexture("../asset/texture/plane_specular.tga");
	plane->SetNormalTexture("../asset/texture/plane_normal.tga");

	plane->Translate(glm::vec3(400.0f, 950.0f, -350.0f));
	plane->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	player = new Player(camera);

	raceRing = new RacingStructure(L"../asset/models/ring.obj");
	raceRing->SetDiffuseTexture("../asset/texture/ring_diffuse.tga");

	CreateRacingRings();

	return TRUE;
}

void OGLWindow::ShadowMapPass()
{
	shadowmapFBO->BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLWindow::CheckCollisions()
{
	if (raceRing->GetCollider().CollidesWith(player->GetCollider()))
	{
		player->SetCurrentRing(player->GetCurrentRing() + 1);
		if (player->GetCurrentRing() > racePointTransforms.size() - 1)
		{
			player->SetCurrentRing(0);
		}
	}
}

void OGLWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindSampler(0, m_texDefaultSampler);

	skybox->SetPosition(*camera->GetCameraPosition());

	// RENDER SKYBOX
	m_skybox_shader->ActivateShaderProgram();
	SetUniforms();
	glDisable(GL_DEPTH_TEST);
	skybox->Render();
	glEnable(GL_DEPTH_TEST);

	m_shader->ActivateShaderProgram();
	shadowmapFBO->BindForReading(GL_TEXTURE4);

	// RENDER TERRAIN
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, glm::vec3(0.0f, 0.0f, 0.0f));
	transformation = glm::scale(transformation, glm::vec3(3.0f, 3.0f, 3.0f));
	SetUniforms();
	terrain->Render();

	// RENDER HOUSES
	//house->Rotate(0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	transformation = house->GetTransformationMatrix();
	SetUniforms();
	house->Render();

	transformation = house_med->GetTransformationMatrix();
	SetUniforms();
	house_med->Render();

	// RENDER THE SUPER SECRET MODEL
	transformation = super_secret->GetTransformationMatrix();
	SetUniforms();
	super_secret->Render();

	// RENDER PLANE
	plane->Rotate(-0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
	plane->Translate(glm::vec3(-80.0f, 0.0f, 0.0f));
	transformation = plane->GetTransformationMatrix();
	SetUniforms();
	plane->Render();

	raceRing->SetTransformationMatrix(racePointTransforms[player->GetCurrentRing()]);
	transformation = raceRing->GetTransformationMatrix();
	SetUniforms();
	raceRing->Render();

	CheckCollisions();
	ShadowMapPass();

	// Camera movement
	camera->DollyCamera(player->GetSpeed());

	// Get and store current cursor position
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int mX = cursorPos.x;
	int mY = cursorPos.y;
	int xDiff = (m_width / 2) - mX;
	int yDiff = (m_height / 2) - mY;
	float camera_smoothing = 0.07f;

	// Yaw and pitch the camera based on the difference the mouse travelled from the centre of the window.
	if (abs(xDiff) > 0 || abs(yDiff) > 0)
	{
		float camYaw = ((float)xDiff * camera_smoothing);
		float camPitch = ((float)yDiff * camera_smoothing);
		camera->RotateCamera(camYaw, camPitch, 0);
	}
	
	// Set the cursor back to the centre of the screen
	SetCursorPos(m_width / 2, m_height / 2);

	HandleKeyDown();

	SwapBuffers(m_hdc);

	return;
}

void OGLWindow::Resize( int width, int height )
{
	float aspect_ratio = (float)width/(float)height;
	camera->SetCameraAspectRatio(aspect_ratio);
	shadowmapFBO->Init(width, height);

	glViewport( 0, 0, width, height );

	glm::mat4 cam_projection = glm::perspective(glm::radians(camera->GetCameraFOV()), camera->GetCameraAspectRatio(),
		0.1f, 50000.0f);

	camera->SetProjectionMatrix(&cam_projection[0][0]);

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	SetVSync(true);

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();
	m_skybox_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_skybox_shader->CreateShaderProgram();
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.vert", SHADER_VERTEX);
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.frag", SHADER_FRAGMENT);

	m_shader->BuildShaderProgram();
	m_shader->ActivateShaderProgram();

	m_skybox_shader->BuildShaderProgram();
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	
	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	return TRUE;
}

void OGLWindow::HandleMouseScroll(int _scroll)
{
	if (_scroll == 120)
	{
		camera->ZoomCamera(5.0f);
	}
	else
	{
		camera->ZoomCamera(-5.0f);
	}
}

void OGLWindow::HandleKeyDown()
{
	//Use windows GetAsyncKeyState to allow multiple key presses at once. (Diagonal movement)
	if (GetAsyncKeyState(W))
	{
		//Dolly
		//camera->DollyCamera(5);
		player->AddSpeed(0.25f);
	}

	if (GetAsyncKeyState(A))
	{
		//Strafe
		camera->StrafeCamera(-5);
	}

	if (GetAsyncKeyState(S))
	{
		//Dolly
		//camera->DollyCamera(-5);
		player->AddSpeed(-0.25);
	}

	if (GetAsyncKeyState(D))
	{
		//Strafe
		camera->StrafeCamera(5);
	}

	if (GetAsyncKeyState(Z))
	{
		//Ped
		camera->PedCamera(5);
	}

	if (GetAsyncKeyState(X))
	{
		//Ped
		camera->PedCamera(-1);
	}

	if (GetAsyncKeyState(Q))
	{
		//Roll
		camera->RotateCamera(0, 0, -1);
	}

	if (GetAsyncKeyState(E))
	{
		//Roll
		camera->RotateCamera(0, 0, 1);
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		// Close Application
		ExitProcess(0);
	}

	if (GetAsyncKeyState(VK_ADD))
		linearAtten -= 0.00025f; // Increase arealight attenuation
	if (GetAsyncKeyState(VK_SUBTRACT))
		linearAtten += 0.00025f; // decrease arealight attenuation
	if (GetAsyncKeyState(VK_F1))	
		ResetRacingRings();
	if (GetAsyncKeyState(VK_F2))
		currentRenderMode = 1;
	if (GetAsyncKeyState(VK_F3))
		currentRenderMode = 2;
	if (GetAsyncKeyState(VK_F4))
		currentRenderMode = 0;
}

void OGLWindow::SetUniforms()
{
	//First rebuild all the matrices
	BuildMatrices();

	// Matrix uniforms
	glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(4, 1, GL_FALSE, &transformation[0][0]);
	glUniformMatrix4fv(5, 1, GL_FALSE, &normal[0][0]);
	glUniform1i(50, currentRenderMode);

	// Vector uniforms
	glm::vec3 campos = *camera->GetCameraPosition();
	glUniform3f(6, campos[0], campos[1], campos[2]); 

	// Lighting uniforms
	glm::vec3 lightDir = directionalLight->GetDirection();
	glm::vec3 lightColor = directionalLight->GetColor();
	glUniform3f(8, lightDir[0], lightDir[1], lightDir[2]);
	glUniform3f(9, lightColor[0], lightColor[1], lightColor[2]);
	glUniform1f(10, directionalLight->GetIntensity());

	glm::vec3 areaPos = areaLight->GetPosition();
	glm::vec3 areaCol = areaLight->GetColor();
	glUniform3f(11, areaPos[0], areaPos[1], areaPos[2]);

	glUniform3f(13, areaCol[0], areaCol[1], areaCol[2]);
	glUniform1f(14, areaLight->GetIntensity());
	glUniform1f(15, areaLight->GetConstAtten());
//	glUniform1f(16, areaLight->GetLinearAtten());
	glUniform1f(16, linearAtten);
	glUniform1f(17, areaLight->GetExpAtten());
}

void OGLWindow::BuildMatrices()
{
	projection = camera->GetProjectionMatrix();
	view = *camera->GetViewMatrix();
	normal = glm::inverseTranspose(transformation);
}

void OGLWindow::SetVSync(bool sync)
{
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(sync);
	}
}

void OGLWindow::CreateRacingRings()
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(1000.0f, 1200.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(1400.0f, 1200.0f, -600.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(1400.0f, 1200.0f, -1600.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(800.0f, 1200.0f, -2000.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 1200.0f, -2000.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-800.0f, 1200.0f, -2000.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-1600.0f, 1200.0f, -1600.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	racePointTransforms.push_back(transform);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-1200.0f, 1200.0f, -800.0f));
	transform = glm::scale(transform, glm::vec3(30.0f, 30.0f, 30.0f));
	transform = glm::rotate(transform, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	racePointTransforms.push_back(transform);

	ResetRacingRings();
}

void OGLWindow::ResetRacingRings()
{
	player->SetCurrentRing(0);
}