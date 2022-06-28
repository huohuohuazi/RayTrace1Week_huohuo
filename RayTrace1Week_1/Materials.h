#pragma once
#ifndef MATERIALS
#define MATERIALS
#include "Material.h"


//���������
class Lambert :public Material {
public:
	Vec3 albedo;//��˥���ʣ�����Ҫ�������ɢ�䣬 ���attenuation=albedo/p
public:
	Lambert(const Vec3& n) :albedo(n) {}

	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
	
};
bool Lambert::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	Vec3 scatter_direction = info.normal + random_unit_vector();
	ray_out = Ray(info.point, scatter_direction);
	attenuation = albedo;
	return true;
}



//��������
class Matal :public Material {
public:
	Vec3 albedo;
public:
	Matal(const Vec3& n) :albedo(n) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
	
};
bool Matal::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	//�����������ѡȡ���򣬾��淴�����Ǿ�����
	Vec3 scatter_direction = reflect(unit_vector(ray_in.direction()), info.normal);
	ray_out = Ray(info.point, scatter_direction);
	attenuation = albedo;
	return dot(ray_out.direction(), info.normal);
}




#endif


