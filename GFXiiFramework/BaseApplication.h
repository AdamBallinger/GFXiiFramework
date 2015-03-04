#pragma once

#include <Windows.h>

class RenderWindow;

//Define BaseApplication as a singleton
class BaseApplication
{
	public:
		enum ERenderSystemType
		{
			RenderSystemD3D11,
			RenderSystemOGL
		};

	private:
		RenderWindow*				m_appwnd;
		static BaseApplication*		s_oglapp;
		BOOL						m_terminate;
		
									BaseApplication();
		virtual						~BaseApplication();
		void						CreateApplicationWindow(int width, int height, ERenderSystemType type );
		BOOL						MyRegisterClass(HINSTANCE hinst);

	public:

		HINSTANCE					m_hInst;

		static BaseApplication*		CreateApplication(HINSTANCE hInst, ERenderSystemType type);
		static void					DestroyApplication();
		static BaseApplication*		GetApplication();

		
		int							Run();
		void						Kill();
		

		inline RenderWindow*			GetApplicationWindow()
		{
			return m_appwnd;
		}

		static	LRESULT CALLBACK	WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};