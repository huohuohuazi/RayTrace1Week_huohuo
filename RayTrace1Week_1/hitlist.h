#pragma once
#ifndef HITLIST
#define HITLIST

#include "Hit.h"
#include<memory>
#include<vector>

//using namespace std;

using std::shared_ptr;//�����б�,�ü���ͼԪ����ʹ��ͬһ��ʵ��
using std::make_shared;

auto double_ptr = make_shared<double>(0.37);
auto vec3_ptr = make_shared<Vec3>(1.414214, 2.718281, 1.618034);



class HitList:public Hit//���������ײ�������б�
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
    bool hit_anything = false;//�Ƿ���ײ������
    double closest_so_far = tmax;

    //������������
    for (const auto& object : objects) {
        if (object->hit(ray_in, tmin, closest_so_far, temp_info)) {
            hit_anything = true;//�����������޸ġ���ײ����Ϊ��
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

    //������������
    for (const auto& object : objects) {
        //�������嶼�Ǵ��ڰ�Χ�еģ��������false�����������
        //����Χ�з��ص�temp��
        if (!object->boundingBox(t0, t1, temp)) return false;
        //������һ�����ӱ��Ϊfirst�����治��first����Ҫ��box�����������İ�Χ��
        box = IsFirstBox ? temp : surroundingBox(box, temp);
        IsFirstBox = false;
    }

    return true;
}

#endif