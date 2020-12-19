#include<Windows.h>
#include<iostream>
#include"platform\Window.h"
#include"Render.h"

//windows program entry point
int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInst,
    LPSTR lpszCmdLine, int nCmdShow)
{
	AllocConsole();
	SetConsoleTitle(L"Debug Window");
	FILE* tmepFile = nullptr;
	freopen_s(&tmepFile, "conout$", "w+t", stdout);

	Window wnd(800,600, "Ray Tracer");
	SoftRender::InitRenderer(wnd.width, wnd.height, wnd.hWnd);
	while (1)
	{
		Window::ProcessMessages();
		{
			SoftRender::UpDate(wnd.hWnd,1);
		}

	}
	
	SoftRender::ShutDown();

	

	return 0;
}
