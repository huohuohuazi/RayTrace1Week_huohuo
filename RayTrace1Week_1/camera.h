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

	double time0, time1;//时刻

public:
	

	//vfov:垂直方向fov，aspect:水平方向长度/竖直方向长度,lens_size:光圈直径,lens_dis:焦距,pos:相机位置，lookat:从pos看向lookat位置，上方向量为vup
	Camera(
		Vec3 pos, Vec3 lookat, Vec3 vup,
		double vfov, double aspect,
		double lens_size,double lens_dis,
		double t0,double t1
	){
		Vec3 u, v, w;//描述相机旋转

		origin = pos;
		lens_radius = lens_size / 2;//光圈半径

		time0 = t0;
		time1 = t1;

		double theta = degrees_to_radians(vfov);
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;//height/width和画布比例一样

		w = unit_vector(pos - lookat);
		u = unit_vector(cross(vup, w));//确保vup始终指向(0,0,1)
		v = cross(w, u);
		
		coner_left_down = origin - half_width * u * lens_dis - half_width * v * lens_dis - lens_dis * w;
		horizontal = 2 * half_width * u * lens_dis;
		vertical = 2 * half_height * v * lens_dis;

		cout << "左下角:" << coner_left_down << endl << "horizontal:" << horizontal << endl << "vertical:" << vertical << endl;

	}


	//在某像素生成一个光线
	Ray getRay(double s,double t)
	{
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(
			origin + offset, 
			coner_left_down + s * horizontal + t * vertical - origin - offset,
			random_double(time0, time1)
		);
	}
};


#endif