#pragma once
#ifndef CAMERA
#define CAMERA

#include "Utils.h"
using namespace std;

//相机，或者说画布类
class Camera {
public:
	Vec3 origin;//相机位置
	Vec3 coner_left_down;//相机左下角位置
	Vec3 horizontal;//x轴向量
	Vec3 vertical;//y轴向量
	Vec3 u, v, w;//相机旋转
	
	double lens_radius;//光圈大小，应该说半径
public:
	//vfov:垂直方向fov，aspect:水平方向长度/竖直方向长度,lens_r:光圈大小,lens_pos:焦距,pos:相机位置，lookat:从pos看向lookat位置，上方向量为vup
	Camera(Vec3 pos, Vec3 lookat, Vec3 vup,double vfov, double aspect,double lens_d,double lens_pos)
	{
		Vec3 u, v, w;//描述相机旋转

		origin = pos;
		lens_radius = lens_d / 2;

		double theta = degrees_to_radians(vfov);
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;//height/width和画布比例一样

		w = unit_vector(pos - lookat);
		u = unit_vector(cross(vup, w));//确保vup始终指向(0,0,1)
		v = cross(w, u);
		
		coner_left_down = origin - half_width * u - half_width * v - lens_pos * w;
		horizontal = 2 * half_width * u * lens_pos;
		vertical = 2 * half_height * v * lens_pos;

		cout << "左下角:" << coner_left_down << endl << "horizontal:" << horizontal << endl << "vertical:" << vertical << endl;

	}

	Camera(Vec3 orig,Vec3 cld,Vec3 hor,Vec3 ver)
	{
		coner_left_down = orig;
		horizontal = cld;
		vertical = hor;
		origin = ver;
		lens_radius = 1.0;
	}
	//默认构造
	Camera()
	{
		lens_radius = 1.0;
		

		coner_left_down = Vec3(-2.0, -1.0, -1.0);
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
		origin = Vec3(0.0, 0.0, 0.0);
		
		u = Vec3(1.0, 0.0, 0.0);
		v = Vec3(0.0, 1.0, 0.0);
		w = Vec3(0.0, 0.0, -1.0);
	}

	//在某像素生成一个光线
	Ray getRay(double s,double t)
	{
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin+offset, coner_left_down + s * horizontal + t * vertical - origin-offset);
	}
};


#endif