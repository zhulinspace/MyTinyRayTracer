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
#include<limits>
float MAXFLOAT = std::numeric_limits<float>::max();
#include<random>
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float>dist(0.0,1.0);
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
//float hit_sphere(const glm::vec3& center, float radius, const Ray& r)
//{
//	glm::vec3 oc = r.origin() - center;
//	float a = glm::dot(r.direction(), r.direction());
//	float b = 2.0 * glm::dot(oc, r.direction());
//	float c = glm::dot(oc, oc) - radius * radius;
//	float discriminant = b * b - 4 * a * c;
//	if (discriminant < 0)
//		return -1.0;
//	else
//		return (-b-sqrt(discriminant)) / (2.0 * a);
//}
glm::vec3 color(const Ray& r,Hitable *world)
{
	hit_record rec;
	
	if (world->hit(r,0.0,MAXFLOAT,rec))
	{
		
		return glm::vec3((rec.normal.x+ 1), (rec.normal.y + 1),(rec.normal.z + 1) )*0.5f;

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
	int ns = 10;
	glm::vec3 lower_left_corner(-2.0, -1.5, -1.0);
	glm::vec3 horizontal(4.0, 0.0, 0.0);
	glm::vec3 vertical(0.0, 3.0, 0.0);
	glm::vec3 origin(0.0, 0.0, 0.0);

	Hitable* list[2];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(glm::vec3(0, -100.5, -1),100);
	Hitable* world = new HitableList(list, 2);
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
				cr += color(r, world);
			}
			cr /= float(ns);
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