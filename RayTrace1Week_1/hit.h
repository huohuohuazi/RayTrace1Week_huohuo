#include "Ray.h"
#include "Materials.h"
#include "AABB.h"

#ifndef HIT
#define HIT

class Material;
class AABB;

struct hit_info
{
	double t;//碰撞的位置
	Vec3 point;//相交点坐标
	Vec3 normal;//法向
	Material* material;//碰撞点材质

	bool Is_frontface;//是否为正面
	
	double u;//uv信息
	double v;//uv信息

	//使得法线始终与入射光方向相反
	inline void set_face_nornal(const Ray& r, const Vec3& outward_normal)
	{
		Is_frontface = dot(r.direction(), outward_normal) < 0;
		normal = Is_frontface ? outward_normal : -outward_normal;
		//normal = Vec3(1, 0, 0);
	}
};


class Hit {
public:
	//碰撞基类
	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const=0;
	//virtual bool hit(const Ray& ray_in, double tmin, double tmax) const = 0;

	//求包围盒
	virtual bool boundingBox(double t0, double t1, AABB& box) const = 0;
	//virtual void getUV(const Vec3& point, double& u, double& v) const = 0;
};


bool overlap(double d, double D, double e, double E, double f, double F)//三个区间
{
	f = max(d, e);
	F = min(D, E);
	return (f < F);
}

#endif
