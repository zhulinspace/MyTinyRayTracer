#include<Windows.h>
#include<iostream>
#include<fstream>
#include"platform\Window.h"
#include"Render.h"
#include"utils\timer.h"
#include<complex>
//windows program entry point
int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInst,
    LPSTR lpszCmdLine, int nCmdShow)
{


	//创建一个窗口
	MyTimer timer;
	Window wnd(800,600, "Ray Tracer");
	SoftRender::InitRenderer(wnd.width, wnd.height, wnd.hWnd);
	while (1)
	{
		Window::ProcessMessages();
		{// Do Frame
			const float t = timer.Peek();
			SoftRender::UpDate(wnd.hWnd,1);
		}

	}
	
	SoftRender::ShutDown();

	

	return 0;
}
