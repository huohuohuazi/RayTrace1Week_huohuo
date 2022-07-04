#include "Ray.h"
#include "Materials.h"

#ifndef HIT
#define HIT

class Material;

struct hit_info
{
	Vec3 point;//�ཻ������
	Vec3 normal;//����
	double t;//��ײ��λ��
	bool Is_frontface;//�Ƿ�Ϊ����
	Material *material;//��ײ�����

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
	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const=0;
	//��������ײ
	
};

#endif
