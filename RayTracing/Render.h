#pragma once
#include<Windows.h>
#include<iostream>
#include<random>
#include"vendor\glm\glm.hpp"

namespace SoftRender
{
	int g_width = 0;
	int g_height = 0;

	HDC g_tempDC = nullptr;
	HBITMAP g_tempBm = nullptr;
	HBITMAP g_oldBm = nullptr;
	unsigned int* g_FrameBuffer = nullptr;
	std::shared_ptr<float[]>g_DepthBuffer = nullptr;

	unsigned int bgColor = ((255 << 16) | (255 << 8) | 255);

	void InitRenderer(int w, int h, HWND hWnd);

	void UpDate(HWND hWnd,const float t);
	void DoOneFrame(const float t);
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

void SoftRender::UpDate(HWND hWnd,const float t)
{
	DoOneFrame(t);

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
			g_FrameBuffer[idx] =bgColor;
			g_DepthBuffer[idx] = 1.0f;
		}
	}
}

void SoftRender::DoOneFrame(const float t)
{
	
	//for (int row = g_height - 1; row >= 0; row--)//为什么改变没有影响
	for(int row=0;row<g_height;row++)
	{
		for (int col = 0; col < g_width; col++)
		{
		
			glm::vec3 color(float(col) / float(g_width), float(row) / float(g_height), 0.2 * t);
			int ir = int(255.99 * color[0]);
			int ig = int(255.99 * color[1]);
			int ib = int(255.99 * color[2]);
			unsigned int newcolor = ((ir << 16) | (ig << 8) | ib);
			int idx = row * g_width + col;
			g_FrameBuffer[idx] = newcolor;
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