#pragma once
#ifndef HITLIST
#define HITLIST

#include "hit.h"
#include<memory>
#include<vector>

//using namespace std;

using std::shared_ptr;//智能列表,让几何图元可以使用同一个实例
using std::make_shared;

auto double_ptr = make_shared<double>(0.37);
auto vec3_ptr = make_shared<Vec3>(1.414214, 2.718281, 1.618034);
auto sphere_ptr = make_shared<Sphere>(Vec3(0, 0, 0), 1.0);


class HitList:public Hit//可与光线碰撞的物体列表
{
public:
	std::vector<shared_ptr<Hit>> objects;
public:
	HitList() {}
	HitList(shared_ptr<Hit> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Hit> object) { objects.push_back(object); }

    virtual bool hit(const Ray& r, double tmin, double tmax, hit_info& info) const;

};

bool HitList::hit(const Ray& r, double t_min, double t_max, hit_info& info) const {
    hit_info temp_info;
    bool hit_anything = false;//是否碰撞到物体
    double closest_so_far = t_max;

    //遍历所有物体
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_info)) {
            hit_anything = true;//碰到物体则修改“碰撞到”为真
            closest_so_far = temp_info.t;
            info = temp_info;
        }
    }
    return hit_anything;
}

#endif