#include "BaseApplication.h"
#include "OpenGL/OGLWindow.h"
#include "D3D11/D3D11Window.h"
#include "Resource.h"
#include <Windowsx.h>

BaseApplication* BaseApplication::s_oglapp = NULL;

BaseApplication::BaseApplication()
{
	m_appwnd = NULL;
	m_hInst = 0;
	m_terminate = FALSE;
}

BaseApplication::~BaseApplication()
{
	if ( m_appwnd )
		delete m_appwnd;
}

BOOL BaseApplication::MyRegisterClass(HINSTANCE hinst)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= this->WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinst;
	wcex.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_OGLWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"RenderWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	if ( !RegisterClassEx ( &wcex ) )
		return FALSE;

	return TRUE;
}

BaseApplication* BaseApplication::CreateApplication(HINSTANCE hinst, ERenderSystemType type)
{
	if ( ! s_oglapp )
	{
		s_oglapp = new BaseApplication();

		s_oglapp->m_hInst = hinst;
		s_oglapp->MyRegisterClass(hinst);

		//Now create an OGLWindow for this application
		s_oglapp->CreateApplicationWindow(1280,720, type);
	}

	return s_oglapp;
}

void BaseApplication::DestroyApplication()
{
	if ( s_oglapp )
		delete s_oglapp;
}

BaseApplication* BaseApplication::GetApplication()
{
	return s_oglapp;
}

void BaseApplication::CreateApplicationWindow( int width, int height, ERenderSystemType type )
{
	if (!m_appwnd)
	{
		if (type == RenderSystemD3D11)
		{
			m_appwnd = new D3D11Window();
			m_appwnd->InitWindow(m_hInst, width, height);
		}
		else if (type == RenderSystemOGL)
		{
			m_appwnd = new OGLWindow();
			m_appwnd->InitWindow(m_hInst, width, height);
		}
		m_appwnd->SetVisible(TRUE);
	}
}

int BaseApplication::Run()
{
	MSG msg;

	while ( !m_terminate )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//peek for windows message
			if ( msg.message == WM_QUIT )
			{
				Kill();
			}
			else
			{
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
		}
		
		m_appwnd->Render();
	}

	return (int) msg.wParam;
}

void BaseApplication::Kill()
{
	m_terminate = TRUE;
}

LRESULT CALLBACK BaseApplication::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int wmId, wmEvent;

	switch ( msg )
	{
		case WM_SIZE:
			s_oglapp->GetApplicationWindow()->Resize( LOWORD(lparam), HIWORD(lparam) );
			break;

		case WM_CLOSE:
			s_oglapp->GetApplicationWindow()->DestroyRenderWindow();
			break;

		case WM_MOUSEMOVE:
			s_oglapp->GetApplicationWindow()->MouseMove( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONUP:
			s_oglapp->GetApplicationWindow()->MouseLBUp( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONDOWN:
			s_oglapp->GetApplicationWindow()->MouseLBDown( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
	}

	return 0;
}