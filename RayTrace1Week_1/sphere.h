#pragma once

#ifndef SPHERE
#define SPHERE

#include "Hit.h"
#include "Vec3.h"

//������Sphere�̳��ڿ���ײ��Hit��
class Sphere : public Hit {
public:
    Vec3 center;//��������
    double radius;//�뾶

public:
    Sphere() {}
    Sphere(Vec3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const Ray& r, double tmin, double tmax, hit_info& rec) const;

    /*void get_uv(const Vec3& p, double& u, double& v)
    {
        double phi = atan2(p.z(), p.x());
        double theta = asin(p.y());
        u = 1 - (phi + pi) / (2 * pi);
        v = (theta + pi / 2) / pi;
    }*/



};

//��ײ��Ϣ������hit_info�У�������ײ�����꣬����ƫ�Ƴ��ȣ���������
bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_info& info) const {
    Vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);

        //�����Դ������Ľ���
        double temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            info.t = temp;
            info.point = r.at(info.t);//��������

            Vec3 outward_normal = (info.point - center) / radius;
            info.set_face_nornal(r, outward_normal);
            return true;
        }
        //�����ԴԶ����Ľ���
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            info.t = temp;
            info.point = r.at(info.t);
            Vec3 outward_normal = (info.point - center) / radius;//������������λ����
            info.set_face_nornal(r, outward_normal);
            return true;
        }
        //std::cout << "����" << std::endl;
    }
    return false;
}
#endif