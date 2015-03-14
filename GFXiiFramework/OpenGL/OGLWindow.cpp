#include "OGLWindow.h"
#include "Resource.h"
#include "../Skybox.h"
#include "GLEW/include/glew.h"

#include <Windows.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>
#include <iostream>

Renderable* plane_mesh;
OGLTexture* plane_texture;
Camera* camera;
Skybox* skybox;

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
	delete camera;
	delete skybox;

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
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
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
	camera->SetCameraAspectRatio((float)width / (float)height);
	camera->SetCameraFOV(70);

	glm::mat4 cam_projection = glm::perspective(glm::radians(camera->GetCameraFOV()), camera->GetCameraAspectRatio(),
		1.0f, 5000.0f);

	camera->SetProjectionMatrix(&cam_projection[0][0]);

	skybox = new Skybox();
	skybox->Init();
	
	m_mesh = new OGLMesh(L"../asset/models/house.obj");

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile( "../asset/texture/house_diffuse.tga");

	m_mesh->SetTexture( m_texture );

	plane_mesh = new OGLMesh(L"../asset/models/space_frigate.obj");
	plane_texture = new OGLTexture();
	plane_texture->CreateTextureFromFile("../asset/texture/tiles_colour.tga");

	plane_mesh->SetTexture(plane_texture);

	return TRUE;
}

void OGLWindow::Render()
{
	float modelview[16];

	Renderable* prenderable = static_cast<Renderable*>(plane_mesh);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//skybox->SetPosition(*camera->GetCameraPosition());
	glDisable(GL_DEPTH_TEST);
	skybox->Render();
	glEnable(GL_DEPTH_TEST);

	glTranslatef( 0.0, 0.0, -100.0 );

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glm::mat4 proj = camera->GetProjectionMatrix();
	glm::mat4 view = *camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	// Create ModelViewProjection matrix
	glm::mat4 mvp = proj * view * model;
	
	glUniformMatrix4fv( m_uniform_modelview, 1, GL_FALSE, modelview);
	glUniformMatrix4fv( m_uniform_projection, 1, GL_FALSE, &mvp[0][0] );

	glBindSampler(0, m_texDefaultSampler);

	prenderable->Render();

	// Get and store current cursor position
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int mX = cursorPos.x;
	int mY = cursorPos.y;
	int xDiff = (m_width / 2) - mX;
	int yDiff = (m_height / 2) - mY;

	// Yaw and pitch the camera based on the difference the mouse travelled from the centre of the window.
	if (xDiff != 0 || yDiff != 0)
	{
		camera->RotateCamera(xDiff, yDiff, 0);
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
		1.0f, 1000.0f);

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

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation( 0, "position" );
	m_shader->BindAttributeLocation( 1, "inNormal" );
	m_shader->BindAttributeLocation( 2, "inUV" );

	glBindFragDataLocation( m_shader->GetProgramHandle(), 0, "outFrag" );

	m_shader->BuildShaderProgram();
	m_shader->ActivateShaderProgram();

	m_uniform_modelview = glGetUniformLocation(m_shader->GetProgramHandle(), "modelview");
	m_uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	m_uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");

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
}
