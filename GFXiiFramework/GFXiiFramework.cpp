// OGLWin32.cpp : Defines the entry point for the application.
//

#include "GFXiiFramework.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <Windows.h>

#include "BaseApplication.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	int exitcode = 0;
	//Allocate a console window
	//so that messages can be redirected to stdout
	AllocConsole();

	HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle( (long) handle_out, _O_TEXT );
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	//Create the application instance
	//To Create a D3D11 render window: replace RenderSystemOGL with RenderSystemD3D11
	BaseApplication* myapp = BaseApplication::CreateApplication(hInstance, 
		BaseApplication::RenderSystemOGL);
	
	exitcode = myapp->Run();
	
	myapp->DestroyApplication();
	
	ExitProcess(0);

	return exitcode;
}
