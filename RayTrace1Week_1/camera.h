#pragma once
#ifndef CAMERA
#define CAMERA

#include "Utils.h"

//���������˵������
class Camera {
public:
	Vec3 origin;
	Vec3 coner_left_down;
	Vec3 horizontal;
	Vec3 vertical;
public:
	Camera(Vec3 orig,Vec3 cld,Vec3 hor,Vec3 ver)
	{
		coner_left_down = orig;
		horizontal = cld;
		vertical = hor;
		origin = ver;
	}
	Camera()
	{
		coner_left_down = Vec3(-2.0, -1.0, -1.0);
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
		origin = Vec3(0.0, 0.0, 0.0);
	}

	//��ĳ��������һ������
	Ray getRay(double u,double v)
	{
		return Ray(origin, coner_left_down + u * horizontal + v * vertical - origin);
	}
};


#endif