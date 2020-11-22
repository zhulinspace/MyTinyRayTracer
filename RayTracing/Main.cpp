#include<Windows.h>
#include<iostream>
#include<fstream>
#include"platform\Window.h"
#include"Render.h"
//windows program entry point
int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInst,
    LPSTR lpszCmdLine, int nCmdShow)
{


	//创建一个窗口
	Window wnd(800, 600, "Ray Tracer");
	SoftRender::InitRenderer(800, 600, wnd.hWnd);
	while (1)
	{
		Window::ProcessMessages();
		{// Do Frame
			SoftRender::UpDate(wnd.hWnd);
		}

	}
	
	SoftRender::ShutDown();

	/*int nx = 200;
	int ny = 100;
	std::ofstream outfile;
	outfile.open("res.ppm");
	if (!outfile) std::cout << "open failed\n";

	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;
			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}*/

	return 0;
}
