#pragma once

#ifndef SPHERE
#define SPHERE

#include "Hit.h"
#include "Vec3.h"
#include "Materials.h"

//������Sphere�̳��ڿ���ײ��Hit��
class Sphere : public Hit {
public:
    Vec3 center;//��������
    double radius;//�뾶
    Material* material;//��ײ�����

public:
    Sphere() {}
    //Sphere(Vec3 cen, double r) { std::cout << "A Simple"; }
    Sphere(Vec3 cen, double r, Material* mat) : center(cen), radius(r), material(mat) { };

    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;

    /*void get_uv(const Vec3& p, double& u, double& v)
    {
        double phi = atan2(p.z(), p.x());
        double theta = asin(p.y());
        u = 1 - (phi + pi) / (2 * pi);
        v = (theta + pi / 2) / pi;
    }*/



};

//��ײ��Ϣ������hit_info�У�������ײ�����꣬����ƫ�Ƴ��ȣ���������
bool Sphere::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const {
    Vec3 oc = ray_in.origin() - center;
    double a = ray_in.direction().length_squared();
    double half_b = dot(oc, ray_in.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);

        //�����Դ������Ľ���
        double temp = (-half_b - root) / a;
        if (temp < tmax && temp > tmin) {
            //����λ��
            info.t = temp;
            info.point = ray_in.at(info.t);//��������
            //����
            Vec3 outward_normal = (info.point - center) / radius;
            info.set_face_nornal(ray_in, outward_normal);

            //������Ϣ
            
            info.material = material;

            return true;
        }
        //�����ԴԶ����Ľ���
        temp = (-half_b + root) / a;
        if (temp < tmax && temp > tmin) {
            info.t = temp;
            info.point = ray_in.at(info.t);
            Vec3 outward_normal = (info.point - center) / radius;//������������λ����
            info.set_face_nornal(ray_in, outward_normal);

            //info.material = make_shared<Material>(Vec3(0,0,0));
            info.material = material;
            return true;
        }
        //std::cout << "����" << std::endl;
    }
    return false;
}
#endif