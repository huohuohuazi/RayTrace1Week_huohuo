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
auto sphere_ptr = make_shared<Sphere>(Vec3(0, 0, 0), 1.0);


class HitList:public Hit//���������ײ�������б�
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
    bool hit_anything = false;//�Ƿ���ײ������
    double closest_so_far = t_max;

    //������������
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_info)) {
            hit_anything = true;//�����������޸ġ���ײ����Ϊ��
            closest_so_far = temp_info.t;
            info = temp_info;
        }
    }
    return hit_anything;
}

#endif