#pragma once
#include<Windows.h>
#include<iostream>
#include<random>
#include"vendor\glm\glm.hpp"
#include"Ray.h"
#include"HitableList.h"
#include"Hitable.h"
#include"Sphere.h"
#include"Camera.h"
#include"Material.h"
#include<limits>
float MAXFLOAT = std::numeric_limits<float>::max();

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
	{ {sizeof(BITMAPINFOHEADER),w,h,1,32,BI_RGB,
		(DWORD)w * h * 4,0,0,0,0}};
	//32指定表示颜色要用到的位数
	g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_FrameBuffer,0,0);
	g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
	g_DepthBuffer.reset(new float[w * h]);
	ClearBuffer();
}

void SoftRender::UpDate(HWND hWnd,const float t)
{
	ClearBuffer();
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



glm::vec3 color(const Ray& r,Hitable *world,int depth)
{
	hit_record rec;
	
	if (world->hit(r,0.001,MAXFLOAT,rec))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return glm::vec3(0, 0, 0);
		}
	}
	else
	{
		glm::vec3 unit_direction = glm::normalize(r.direction());
		float t = 0.5 * (unit_direction.y + 1.0);
		return  glm::vec3(1.0, 1.0, 1.0) * (1 - t) + t * glm::vec3(0.5, 0.7, 1.0);
	}
	

}

void SoftRender::DoOneFrame(const float t)
{
	int ns =10;

	Hitable* list[4];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5,new lambertian(glm::vec3(0.8,0.3,0.3)));
	list[1] = new Sphere(glm::vec3(0, -100.5, -1),100,new lambertian(glm::vec3(0.8,0.8,0.0)));
	list[2] = new Sphere(glm::vec3(1,0, -1), 0.5, new metal(glm::vec3(0.8, 0.6, 0.2),0.3));
	list[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5, new metal(glm::vec3(0.8, 0.8, 0.8),0.3));
	Hitable* world = new HitableList(list,4);
	Camera cam;
	//for(int row= g_height-1;row>=0;row--)
	for(int row=0;row<g_height;row++)
	{
		for (int col = 0; col < g_width; col++)
		{
			glm::vec3 cr(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(col+dist(mt)) / float(g_width);
				float v = float(row+ dist(mt)) / float(g_height);
				Ray r = cam.GetRay(u, v);
				cr += color(r, world,0);
			}
			cr /= float(ns);
			cr = glm::vec3(sqrtf(cr[0]), sqrtf(cr[1]), sqrtf(cr[2]));
			int ir = int(255.99 * cr[0]);
			int ig = int(255.99 * cr[1]);
			int ib = int(255.99 * cr[2]);
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