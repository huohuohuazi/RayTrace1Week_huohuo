
#ifndef MATERIALS
#define MATERIALS
#include "Material.h"
#include "Texture.h"

double schlick(double cosine, double ref_idx);

#pragma region 漫反射
//漫反射材质
class Lambert :public Material {
public:
	Texture *albedo;//以前是反射指定的颜色，现在通过材质影响
	 
public:
	Lambert(Texture* a) :albedo(a) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
};
bool Lambert::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	//漫反射方向
	Vec3 scatter_direction = info.normal + random_unit_vector();
	ray_out = Ray(info.point, scatter_direction,ray_in.time());
	//反射效果
	attenuation = albedo->value(info.u, info.v, info.point);//获取该点的颜色信息
	return true;
}
#pragma endregion


#pragma region 金属
//金属材质
class Matal :public Material {
public:
	Vec3 albedo;
	double fuzz;//金属粗糙度

public:
	Matal(const Vec3& n, double fu) :albedo(n), fuzz(fu < 1 ? fu : 1) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
	
};
bool Matal::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	//漫反射是随机选取方向，镜面反射则是镜像反射
	Vec3 scatter_direction = reflect(unit_vector(ray_in.direction()), info.normal);
	ray_out = Ray(info.point, scatter_direction+fuzz*random_in_unit_sphere());
	attenuation = albedo;
	return (dot(ray_out.direction(), info.normal) > 0);
}
#pragma endregion

#pragma region 绝缘体
//金属材质
class Dielec :public Material {
public:
	double eta;//介质折射率

public:
	Dielec(double ri) :eta(ri) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;

};
bool Dielec::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	attenuation = Vec3(1.0, 1.0, 1.0);
	//需要区分射出与摄入光线
	double temp_eta = info.Is_frontface ? 1.0 / eta : eta;

	//入射角信息
	Vec3 ray_in_direction = unit_vector(ray_in.direction());

	double cos_theta = ffmin(dot(-ray_in_direction, info.normal),1);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	//从介质中射出的全反射
	if (temp_eta * sin_theta > 1.0) {
		Vec3 ray_out_direction = reflect(ray_in_direction, info.normal);
		ray_out = Ray(info.point, ray_out_direction);
		return true;
	}
	
	double real_eta = schlick(cos_theta, temp_eta);
	//反射
	if (real_eta > random_double())
	{
		Vec3 ray_out_direction = reflect(ray_in_direction, info.normal);
		ray_out = Ray(info.point, ray_out_direction);
		return true;
	}
	//折射
	Vec3 ray_out_direction = refract(ray_in_direction, info.normal, temp_eta);
	ray_out = Ray(info.point, ray_out_direction);
	return true;
}

#pragma endregion

//折射率的近似公式，因为现实中玻璃的折射率与入射角有关系
//原来叫菲涅尔项
double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}


#endif


