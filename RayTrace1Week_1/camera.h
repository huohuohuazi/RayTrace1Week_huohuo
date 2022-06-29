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
	//vfov:��ֱ����fov��aspect:ˮƽ���򳤶�/��ֱ���򳤶�,lens_size:��Ȧֱ��,lens_dis:����,pos:���λ�ã�lookat:��pos����lookatλ�ã��Ϸ�����Ϊvup
	Camera(Vec3 pos, Vec3 lookat, Vec3 vup,double vfov, double aspect,double lens_size,double lens_dis)
	{
		Vec3 u, v, w;//���������ת

		origin = pos;
		lens_radius = lens_size / 2;//��Ȧ�뾶

		double theta = degrees_to_radians(vfov);
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;//height/width�ͻ�������һ��

		w = unit_vector(pos - lookat);
		u = unit_vector(cross(vup, w));//ȷ��vupʼ��ָ��(0,0,1)
		v = cross(w, u);
		
		coner_left_down = origin - half_width * u * lens_dis - half_width * v * lens_dis - lens_dis * w;
		horizontal = 2 * half_width * u * lens_dis;
		vertical = 2 * half_height * v * lens_dis;

		cout << "���½�:" << coner_left_down << endl << "horizontal:" << horizontal << endl << "vertical:" << vertical << endl;

	}

	//Ĭ�Ϲ���
	Camera()
	{
		std::cout << "Ĭ�Ϲ��캯��" << std::endl;
	}

	//��ĳ��������һ������
	Ray getRay(double s,double t)
	{
		Vec3 rd = lens_radius * random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, coner_left_down + s * horizontal + t * vertical - origin - offset);
	}
};


#endif