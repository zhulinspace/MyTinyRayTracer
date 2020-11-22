#pragma once
#include<Windows.h>
#include<iostream>
namespace SoftRender
{
	int g_width = 0;
	int g_height = 0;

	HDC g_tempDC = nullptr;
	HBITMAP g_tempBm = nullptr;
	HBITMAP g_oldBm = nullptr;
	unsigned int* g_FrameBuffer = nullptr;
	std::shared_ptr<float[]>g_DepthBuffer = nullptr;

	unsigned int bgColor = ((123 << 16) | (195 << 8) | 221);

	void InitRenderer(int w, int h, HWND hWnd);

	void UpDate(HWND hWnd);

	void ClearBuffer();
	void ShutDown();

}

void SoftRender::InitRenderer(int w, int h, HWND hWnd)
{
	g_width = w;
	g_height = h;

	HDC hDC = GetDC(hWnd);
	g_tempDC = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	BITMAPINFO bi =
	{ {sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,
		(DWORD)w * h * 4,0,0,0,0}};
	g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_FrameBuffer,0,0);
	g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
	g_DepthBuffer.reset(new float[w * h]);
	ClearBuffer();
}

void SoftRender::UpDate(HWND hWnd)
{
	ClearBuffer();

	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, g_width, g_height, g_tempDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd,hDC);
}

void SoftRender::ClearBuffer()
{
	for (int row = 0; row < g_height; row++)
	{
		for (int col = 0; col < g_width; col++)
		{
			int idx = row * g_width + col;
			g_FrameBuffer[idx] = bgColor;
			g_DepthBuffer[idx] = 1.0f;
		}
	}
}

void SoftRender::ShutDown()
{
	if (g_tempDC)
	{
		if (g_tempBm)
		{
			SelectObject(g_tempDC, g_oldBm);
			g_oldBm = nullptr;
		}

		DeleteDC(g_tempDC);
		g_tempDC = nullptr;
	}

	if (g_tempBm)
	{
		DeleteObject(g_tempBm);
		g_tempBm = nullptr;
	}
}