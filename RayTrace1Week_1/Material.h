#pragma once
#ifndef MATERIAL
#define MATERIAL
#include "Hit.h"


class Material {
public:
	//���ط�����Ϣ
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const = 0;
	

};


#endif