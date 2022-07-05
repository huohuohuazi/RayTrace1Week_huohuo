#pragma once
#ifndef HITLIST
#define HITLIST

#include "Hit.h"
#include<memory>
#include<vector>

//using namespace std;

using std::shared_ptr;//智能列表,让几何图元可以使用同一个实例
using std::make_shared;

auto double_ptr = make_shared<double>(0.37);
auto vec3_ptr = make_shared<Vec3>(1.414214, 2.718281, 1.618034);



class HitList:public Hit//可与光线碰撞的物体列表
{
public:
	std::vector<shared_ptr<Hit>> objects;
public:
	HitList() {}
	HitList(shared_ptr<Hit> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Hit> object) { objects.push_back(object); }

    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
    virtual bool boundingBox(double t0, double t1, AABB& box) const;
};

bool HitList::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const {
    hit_info temp_info;
    bool hit_anything = false;//是否碰撞到物体
    double closest_so_far = tmax;

    //遍历所有物体
    for (const auto& object : objects) {
        if (object->hit(ray_in, tmin, closest_so_far, temp_info)) {
            hit_anything = true;//碰到物体则修改“碰撞到”为真
            closest_so_far = temp_info.t;
            info = temp_info;
        }
    }
    return hit_anything;
}

bool HitList::boundingBox(double t0, double t1, AABB& box) const
{
    if (objects.empty()) return false;

    AABB temp;
    bool IsFirstBox = true;

    //遍历所有物体
    for (const auto& object : objects) {
        //正常物体都是存在包围盒的，如果返回false则是特殊情况
        //将包围盒返回到temp中
        if (!object->boundingBox(t0, t1, temp)) return false;
        //碰到第一个盒子标记为first，后面不是first，需要将box迭代以求最大的包围盒
        box = IsFirstBox ? temp : surroundingBox(box, temp);
        IsFirstBox = false;
    }

    return true;
}

#endif