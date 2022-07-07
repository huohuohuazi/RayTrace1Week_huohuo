#pragma once
#ifndef MATERIAL
#define MATERIAL

#include "Hit.h"

class Material {
public:
	//返回反射信息
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const = 0;
	//返回发光信息
	virtual Vec3 emit(double u, double v, const Vec3& point) const
	{
		return Vec3(0, 0, 0);
	}
};



//用来反转材质正反
class Flipface : public Hit
{
public:
	shared_ptr<Hit> ptr;
public:
	Flipface(shared_ptr<Hit> p) : ptr(p) {}

	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
	virtual bool boundingBox(double t0, double t1, AABB& box) const;

};
bool Flipface::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
	if (!ptr->hit(ray_in, tmin, tmax, info))
		return false;

	info.Is_frontface = !info.Is_frontface;
	return true;
}
bool Flipface::boundingBox(double t0, double t1, AABB& box) const
{
	return ptr->boundingBox(t0, t1, box);
}

#endif