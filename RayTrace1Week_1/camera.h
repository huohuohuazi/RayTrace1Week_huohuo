#pragma once
#ifndef CAMERA
#define CAMERA

#include "Utils.h"
using namespace std;

//���������˵������
class Camera {
public:
	Vec3 origin;//���λ��
	Vec3 coner_left_down;//������½�λ��
	Vec3 horizontal;//x������
	Vec3 vertical;//y������
	Vec3 u, v, w;//�����ת
	
	double lens_radius;//��Ȧ��С��Ӧ��˵�뾶
public:
	//vfov:��ֱ����fov��aspect:ˮƽ���򳤶�/��ֱ���򳤶�,lens_r:��Ȧ��С,lens_pos:����,pos:���λ�ã�lookat:��pos����lookatλ�ã��Ϸ�����Ϊvup
	Camera(Vec3 pos, Vec3 lookat, Vec3 vup,double vfov, double aspect,double lens_d,double lens_pos)
	{
		Vec3 u, v, w;//���������ת

		origin = pos;
		lens_radius = lens_d / 2;

		double theta = degrees_to_radians(vfov);
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;//height/width�ͻ�������һ��

		w = unit_vector(pos - lookat);
		u = unit_vector(cross(vup, w));//ȷ��vupʼ��ָ��(0,0,1)
		v = cross(w, u);
		
		coner_left_down = origin - half_width * u - half_width * v - lens_pos * w;
		horizontal = 2 * half_width * u * lens_pos;
		vertical = 2 * half_height * v * lens_pos;

		cout << "���½�:" << coner_left_down << endl << "horizontal:" << horizontal << endl << "vertical:" << vertical << endl;

	}

	Camera(Vec3 orig,Vec3 cld,Vec3 hor,Vec3 ver)
	{
		coner_left_down = orig;
		horizontal = cld;
		vertical = hor;
		origin = ver;
		lens_radius = 1.0;
	}
	//Ĭ�Ϲ���
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

	//��ĳ��������һ������
	Ray getRay(double s,double t)
	{
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin+offset, coner_left_down + s * horizontal + t * vertical - origin-offset);
	}
};


#endif