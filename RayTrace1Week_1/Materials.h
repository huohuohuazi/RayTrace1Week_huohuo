
#ifndef MATERIALS
#define MATERIALS
#include "Material.h"

double schlick(double cosine, double ref_idx);

#pragma region ������
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
#pragma endregion

#pragma region ����
//��������
class Matal :public Material {
public:
	Vec3 albedo;
	double fuzz;//�����ֲڶ�

public:
	Matal(const Vec3& n, double fu) :albedo(n), fuzz(fu < 1 ? fu : 1) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;
	
};
bool Matal::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	//�����������ѡȡ���򣬾��淴�����Ǿ�����
	Vec3 scatter_direction = reflect(unit_vector(ray_in.direction()), info.normal);
	ray_out = Ray(info.point, scatter_direction+fuzz*random_in_unit_sphere());
	attenuation = albedo;
	return (dot(ray_out.direction(), info.normal) > 0);
}
#pragma endregion

#pragma region ��Ե��
//��������
class Dielec :public Material {
public:
	double eta;//����������

public:
	Dielec(double ri) :eta(ri) {}
	virtual bool scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const;

};
bool Dielec::scatter(const Ray& ray_in, hit_info& info, Vec3& attenuation, Ray& ray_out) const
{
	attenuation = Vec3(1.0, 1.0, 1.0);
	//��Ҫ����������������
	double temp_eta = info.Is_frontface ? 1.0 / eta : eta;

	//�������Ϣ
	Vec3 ray_in_direction = unit_vector(ray_in.direction());

	double cos_theta = ffmin(dot(-ray_in_direction, info.normal),1);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	//�ӽ����������ȫ����
	if (temp_eta * sin_theta > 1.0) {
		Vec3 ray_out_direction = reflect(ray_in_direction, info.normal);
		ray_out = Ray(info.point, ray_out_direction);
		return true;
	}
	
	double real_eta = schlick(cos_theta, temp_eta);
	//����
	if (real_eta > random_double())
	{
		Vec3 ray_out_direction = reflect(ray_in_direction, info.normal);
		ray_out = Ray(info.point, ray_out_direction);
		return true;
	}
	//����
	Vec3 ray_out_direction = refract(ray_in_direction, info.normal, temp_eta);
	ray_out = Ray(info.point, ray_out_direction);
	return true;
}

#pragma endregion

//�����ʵĽ��ƹ�ʽ����Ϊ��ʵ�в�������������������й�ϵ
//ԭ���з�������
double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}


#endif


