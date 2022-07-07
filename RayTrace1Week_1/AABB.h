#ifndef AABB_H
#define AABB_H

#include "Utils.h"
#include "Hit.h"
#include "Vec3.h"

//包围盒
class AABB {
public:
	Vec3 _min;
	Vec3 _max;
public:
	AABB(){}
	AABB(const Vec3 &a, const Vec3 &b):_min(a),_max(b){}

	Vec3 minNum() const { return _min; }
	Vec3 maxNum() const { return _max; }

	//专门用于包围盒求交，因此不继承Hit类方法
	bool hit(const Ray& ray_in, double tmin, double tmax) const
	{
		for (int a = 0; a < 3; a++)
		{
			//简化了一些计算，运算的结果为判定光线是否与判定盒碰撞，仅根据三个维度的边界信息进行计算
			//即与长方体求交
			double invD = 1.0 / ray_in.direction()[a];
			double t0 = (_min[a] - ray_in.origin()[a]) * invD;
			double t1 = (_max[a] - ray_in.origin()[a]) * invD;

			//t-=tan(theta)=与该方向最负位置点连线/
			if (invD < 0) std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin) return false;
		}
		return true;
	}

};

//求包围盒的包围盒
inline AABB surroundingBox(AABB box0, AABB box1)
{
	Vec3 smaller
	(ffmin(box0.minNum().x(), box1.minNum().x()),
		ffmin(box0.minNum().y(), box1.minNum().y()),
		ffmin(box0.minNum().z(), box1.minNum().z()));
	Vec3 bigger
	(ffmax(box0.maxNum().x(), box1.maxNum().x()),
		ffmax(box0.maxNum().y(), box1.maxNum().y()),
		ffmax(box0.maxNum().z(), box1.maxNum().z()));
	return AABB(smaller, bigger);
}



#endif