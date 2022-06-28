#pragma once
#ifndef MATERIALS
#define MATERIALS
#include "Material.h"


//漫反射材质
class Lambert :public Material {
public:
	Vec3 albedo;//总衰减率，由于要进行随机散射， 因此attenuation=albedo/p
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



//金属材质
class Matal :public Material {
public:
	Vec3 albedo;
public:
	Matal(const Vec3& n) :albedo(n) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
	
};
bool Matal::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	//漫反射是随机选取方向，镜面反射则是镜像反射
	Vec3 scatter_direction = reflect(unit_vector(ray_in.direction()), info.normal);
	ray_out = Ray(info.point, scatter_direction);
	attenuation = albedo;
	return dot(ray_out.direction(), info.normal);
}




#endif


