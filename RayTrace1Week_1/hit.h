#include "Ray.h"
#include "Materials.h"
#include "AABB.h"

#ifndef HIT
#define HIT

class Material;
class AABB;

struct hit_info
{
	double t;//��ײ��λ��
	Vec3 point;//�ཻ������
	Vec3 normal;//����
	Material* material;//��ײ�����

	bool Is_frontface;//�Ƿ�Ϊ����
	
	double u;//uv��Ϣ
	double v;//uv��Ϣ

	//ʹ�÷���ʼ��������ⷽ���෴
	inline void set_face_nornal(const Ray& r, const Vec3& outward_normal)
	{
		Is_frontface = dot(r.direction(), outward_normal) < 0;
		normal = Is_frontface ? outward_normal : -outward_normal;
		//normal = Vec3(1, 0, 0);
	}
};


class Hit {
public:
	//��ײ����
	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const=0;
	//virtual bool hit(const Ray& ray_in, double tmin, double tmax) const = 0;

	//���Χ��
	virtual bool boundingBox(double t0, double t1, AABB& box) const = 0;
};


bool overlap(double d, double D, double e, double E, double f, double F)//��������
{
	f = max(d, e);
	F = min(D, E);
	return (f < F);
}

#endif
