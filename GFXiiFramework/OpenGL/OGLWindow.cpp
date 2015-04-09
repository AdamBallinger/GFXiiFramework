#include "OGLWindow.h"
#include "Resource.h"
#include "GLEW/include/glew.h"

#include <Windows.h>
#include <iostream>

OGLWindow::OGLWindow()
{
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_mesh;
	delete plane_mesh;
	delete m_shader;
	delete m_skybox_shader;
	delete camera;
	delete skybox;
	delete terrain;

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
	camera->SetCameraPosition(&glm::vec3(0.0f, 20.0f, 0.0f));
	camera->SetCameraAspectRatio((float)width / (float)height);
	camera->SetCameraFOV(70);

	glm::mat4 cam_projection = glm::perspective(glm::radians(camera->GetCameraFOV()), camera->GetCameraAspectRatio(),
		0.1f, 5000.0f);

	camera->SetProjectionMatrix(&cam_projection[0][0]);

	directionalLight = new DirectionalLight();
	directionalLight->SetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
	directionalLight->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	directionalLight->SetIntensity(1.0f);

	areaLight = new AreaLight();
	areaLight->SetPosition(glm::vec3(10.0f, 0.0f, -40.0f));
	areaLight->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
	areaLight->SetIntensity(0.7f);
	areaLight->SetConstAtten(0.3f);
	areaLight->SetLinearAtten(0.007f);
	areaLight->SetExpAtten(0.0008f);

	spotLight = new SpotLight();
	spotLight->SetPosition(glm::vec3(20.0f, 3.0f, -90.0f));
	spotLight->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	spotLight->SetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
	spotLight->SetIntensity(1.5f);
	spotLight->SetExponent(0.3f);
	spotLight->SetCutOff(15.0);

	skybox = new Skybox();
	skybox->Init();

	terrain = new Terrain();
	terrain->CreateTerrainFromHeightMap("../asset/texture/terrain.tga");
	
	m_mesh = new OGLMesh(L"../asset/models/house.obj");

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile( "../asset/texture/house_diffuse.tga");
	m_mesh->SetTexture( m_texture );
	
	m_specularTexture = new OGLTexture();
	m_specularTexture->CreateTextureFromFile("../asset/texture/house_spec.tga");
	m_mesh->SetSpecTexture(m_specularTexture);

	m_normalTexture = new OGLTexture();
	m_normalTexture->CreateTextureFromFile("../asset/texture/house_normal.tga");
	m_mesh->SetNormalTexture(m_normalTexture);

	plane_mesh = new OGLMesh(L"../asset/models/space_frigate.obj");
	plane_texture = new OGLTexture();
	plane_texture->CreateTextureFromFile("../asset/texture/tiles_colour.tga");

	plane_mesh->SetTexture(plane_texture);

	return TRUE;
}
float angle = 0.0f;
void OGLWindow::Render()
{
	Renderable* prenderable = static_cast<Renderable*>(m_mesh);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glBindSampler(0, m_texDefaultSampler);

	skybox->SetPosition(*camera->GetCameraPosition());

	angle += .3f;

	// RENDER SKYBOX
	m_skybox_shader->ActivateShaderProgram();
	SetUniforms();
	glDisable(GL_DEPTH_TEST);
	skybox->Render();
	glEnable(GL_DEPTH_TEST);

	// RENDER HOUSE
	m_shader->ActivateShaderProgram();
	glTranslatef(20.0f, 0, -70.0f);
	glScalef(2, 2, 2);
	glRotatef(angle, 0, 1, 0);
	SetUniforms();
	prenderable->Render();

	// Get and store current cursor position
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int mX = cursorPos.x;
	int mY = cursorPos.y;
	int xDiff = (m_width / 2) - mX;
	int yDiff = (m_height / 2) - mY;
	float camera_smoothing = 0.2f;

	// Yaw and pitch the camera based on the difference the mouse travelled from the centre of the window.
	if (abs(xDiff) > 0 || abs(yDiff) > 0)
	{
		camera->RotateCamera(xDiff * camera_smoothing, yDiff * camera_smoothing, 0);
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

	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glm::mat4 cam_projection = glm::perspective(glm::radians(camera->GetCameraFOV()), camera->GetCameraAspectRatio(),
		0.1f, 5000.0f);

	camera->SetProjectionMatrix(&cam_projection[0][0]);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();
	m_skybox_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_skybox_shader->CreateShaderProgram();
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.vert", SHADER_VERTEX);
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation( 0, "position" );
	m_shader->BindAttributeLocation( 1, "inNormal" );
	m_shader->BindAttributeLocation(2, "inUV");

	m_skybox_shader->BindAttributeLocation(0, "position");
	m_skybox_shader->BindAttributeLocation(1, "inNormal");
	m_skybox_shader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_skybox_shader->GetProgramHandle(), 0, "outFrag");

	glBindFragDataLocation( m_shader->GetProgramHandle(), 0, "outFrag" );

	m_shader->BuildShaderProgram();
	m_shader->ActivateShaderProgram();

	m_skybox_shader->BuildShaderProgram();

	glUniform1i( m_uniform_texture, 0 );
	
	//Create a texture sampler
	glGenSamplers( 1, (GLuint*)(&m_texDefaultSampler) );
	
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MIN_FILTER , GL_LINEAR);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MAG_FILTER , GL_LINEAR);

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
		camera->DollyCamera(1);
	}

	if (GetAsyncKeyState(A))
	{
		//Strafe
		camera->StrafeCamera(-1);
	}

	if (GetAsyncKeyState(S))
	{
		//Dolly
		camera->DollyCamera(-1);
	}

	if (GetAsyncKeyState(D))
	{
		//Strafe
		camera->StrafeCamera(1);
	}

	if (GetAsyncKeyState(Z))
	{
		//Ped
		camera->PedCamera(1);
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
}

void OGLWindow::SetUniforms()
{
	//First rebuild all the matrices
	BuildMatrices();

	// Matrix uniforms
	glUniformMatrix4fv(0, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(1, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(5, 1, GL_FALSE, &normal[0][0]);

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
	glUniform3f(17, areaPos[0], areaPos[1], areaPos[2]);
	glUniform3f(19, areaCol[0], areaCol[1], areaCol[2]);
	glUniform1f(20, areaLight->GetIntensity());
	glUniform1f(21, areaLight->GetConstAtten());
	glUniform1f(22, areaLight->GetLinearAtten());
	glUniform1f(23, areaLight->GetExpAtten());

	//glm::vec3 spotPos = spotLight->GetPosition();
	//glm::vec3 spotCol = spotLight->GetColor();
	//glm::vec3 spotDir = spotLight->GetDirection();
	//glUniform3f(10, spotPos[0], spotPos[1], spotPos[2]);
	//glUniform3f(11, spotCol[0], spotCol[1], spotCol[2]);
	//glUniform3f(12, spotDir[0], spotDir[1], spotDir[2]);
	//glUniform1f(13, spotLight->GetIntensity());
	//glUniform1f(14, spotLight->GetExponent());
	//glUniform1f(15, spotLight->GetCutOff());
}


void OGLWindow::BuildMatrices()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	projection = camera->GetProjectionMatrix();
	view = *camera->GetViewMatrix();
	model = glm::mat4(1.0f);
	normal = glm::inverseTranspose(glm::make_mat4(modelview));

	// Create ModelViewProjection matrix
	MVP = projection * view * model;
}