#ifndef MOVINGSPHERE
#define MOVINGSPHERE

#include "hit.h"

//�ƶ�������
class Moving_sphere : public Hit {
public:
    Vec3 center0, center1;
    double time0, time1;
    double radius;
    Material* material;

public:
    Moving_sphere() {}
    Moving_sphere(
        double r,Vec3 cen0, Vec3 cen1, 
        double t0, double t1, Material* m)
        :center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(m){};

    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;

    Vec3 position(double time) const;//��ĳһʱ�����ٶ�

};

Vec3 Moving_sphere::position(double time) const {
    Vec3 speed = (center1 - center0) / (time1 - time0);//�ٶ�
    //std::cout << speed << std::endl;
    return (center0 + (time - time0) * speed);
}

//���壬��ʼ�ٶ�Ϊspeed = (center1 - center0) / (time1 - time0)����������ʱ��λ��Ϊ position(ray_in.time()
bool Moving_sphere::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
    //������ray_in.time�䵽�������
    Vec3 oc = ray_in.origin() - position(ray_in.time());//�����߶�

    double a = ray_in.direction().length_squared();
    double half_b = dot(oc, ray_in.direction());
    double c = oc.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;//��

    //����н�
    if (discriminant > 0) {
        double root = sqrt(discriminant);

        double temp = (-half_b - root) / a;//����Ľ�
        if (temp < tmax && temp > tmin) {

            info.t = temp;
            info.point = ray_in.at(info.t);

            Vec3 outward_normal = (info.point - position(ray_in.time())) / radius;
            info.set_face_nornal(ray_in, outward_normal);
            info.material = material;
            return true;
        }

        //��һ������
        temp = (-half_b + root) / a;

        if (temp < tmax && temp > tmin) {
            info.t = temp;
            info.point = ray_in.at(info.t);
            Vec3 outward_normal = (info.point - position(ray_in.time())) / radius;
            info.set_face_nornal(ray_in, outward_normal);
            info.material = material;
            return true;
        }
    }
    return false;
}



#endif