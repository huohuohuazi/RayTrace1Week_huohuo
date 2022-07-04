#include "Ray.h"
#include "Materials.h"

#ifndef HIT
#define HIT

class Material;

struct hit_info
{
	Vec3 point;//相交点坐标
	Vec3 normal;//法向
	double t;//碰撞的位置
	bool Is_frontface;//是否为正面
	Material *material;//碰撞点材质

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
	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const=0;
	//与球体碰撞
	
};

#endif
