#pragma once
#include<Windows.h>
#include<iostream>
#include<fstream>
#include"utils\utility.h"

#include"HitableList.h"
#include"bvh.h"
#include"Sphere.h"
#include"Camera.h"
#include"Material.h"

//#include"bvh.h"


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



vec3 color(const Ray& r,Hitable &world,int depth)
{
	hit_record rec;
	
	if (depth <= 0)
		return vec3(0, 0, 0);
	if (world.hit(r,0.001, infinity,rec))
	{
		Ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth -1);
		}
		return vec3(0, 0, 0);
	} 
	
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return  vec3(1.0, 1.0, 1.0) * (1 - t) + t * vec3(0.5, 0.7, 1.0);
	
	

}

HitableList random_scene()
{
	HitableList list;

	auto material_ground = make_shared<lambertian>(vec3(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(vec3(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.0);

	list.add(make_shared<Sphere>(vec3(0.0, -100.5, -1.0), 100.0, material_ground));
	list.add(make_shared<Sphere>(vec3(0.0, 0.0, -1.0), 0.5, material_center));
	list.add(make_shared<Sphere>(vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	list.add(make_shared<Sphere>(vec3(-1.0, 0.0, -1.0), -0.45, material_left));
	list.add(make_shared<Sphere>(vec3(1.0, 0.0, -1.0), 0.5, material_right));

	//return HitableList(make_shared<bvh_node>(list));
	return list;
}


void SoftRender::DoOneFrame(const float t)
{
	std::ofstream outputfile;
	outputfile.open("res.txt", std::ios::app);
	MyTimer timer;

	int ns =100;
	int max_depth=50;
	
	
	HitableList world = random_scene();
	

	
	
	double time =timer.Mark() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
	outputfile << "construct time:"<<time << " seconds" << "\n";
	


	double aspect = double(g_width*1.0f / g_height);

	Camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, aspect);

	//for(int row= g_height-1;row>=0;row--)
	for(int row=0;row<g_height;row++)
	{
		for (int col = 0; col < g_width; col++)
		{
			vec3 cr(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++)
			{
				auto u = double(col+random_double()) / double(g_width);
				auto v = double(row+ random_double()) / double(g_height);
				Ray r = cam.GetRay(u, v);
				cr += color(r, world, max_depth);
			}
			auto scale = 1.0 / double(ns);
			cr *= scale;
			cr = vec3(sqrt(cr[0]), sqrt(cr[1]), sqrt(cr[2]));
			int ir = int(255.99 * cr[0]);
			int ig = int(255.99 * cr[1]);
			int ib = int(255.99 * cr[2]);
			unsigned int newcolor = ((ir << 16) | (ig << 8) | ib);
			int idx = row * g_width + col;
			g_FrameBuffer[idx] = newcolor;
		}
	}

	time = timer.Mark() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
	outputfile <<"rendering time:" <<time << " seconds" << "\n";
	outputfile.close();
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