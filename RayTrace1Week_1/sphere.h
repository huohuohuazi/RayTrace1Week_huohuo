#pragma once

#ifndef SPHERE
#define SPHERE

#include "Hit.h"
#include "Vec3.h"
#include "Materials.h"

//球体类Sphere继承于可碰撞类Hit类
class Sphere : public Hit {
public:
    Vec3 center;//中心坐标
    double radius;//半径
    Material* material;//碰撞点材质

public:
    Sphere() {}
    //Sphere(Vec3 cen, double r) { std::cout << "A Simple"; }
    Sphere(Vec3 cen, double r, Material* mat) : center(cen), radius(r), material(mat) { };

    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
    virtual bool boundingBox(double t0, double t1, AABB& box) const;
    virtual void getUV(const Vec3& point, double& u, double& v) const;

    /*void get_uv(const Vec3& p, double& u, double& v)
    {
        double phi = atan2(p.z(), p.x());
        double theta = asin(p.y());
        u = 1 - (phi + pi) / (2 * pi);
        v = (theta + pi / 2) / pi;
    }*/

    

};

//碰撞信息保存于hit_info中，包括碰撞点坐标，射线偏移长度，法线向量
bool Sphere::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const {
    Vec3 oc = ray_in.origin() - center;
    double a = ray_in.direction().length_squared();
    double half_b = dot(oc, ray_in.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);

        //距离光源近距离的交点
        double temp = (-half_b - root) / a;
        if (temp < tmax && temp > tmin) {
            //交点位置
            info.t = temp;
            info.point = ray_in.at(info.t);//交点坐标
            //法线
            Vec3 outward_normal = (info.point - center) / radius;
            info.set_face_nornal(ray_in, outward_normal);

            //材质信息
            getUV((info.point - center) / radius, info.u, info.v);
            info.material = material;

            return true;
        }
        //距离光源远距离的交点
        temp = (-half_b + root) / a;
        if (temp < tmax && temp > tmin) {
            info.t = temp;
            info.point = ray_in.at(info.t);
            Vec3 outward_normal = (info.point - center) / radius;//法向向量（单位化）
            info.set_face_nornal(ray_in, outward_normal);

            //info.material = make_shared<Material>(Vec3(0,0,0));
            info.material = material;
            getUV((info.point - center) / radius, info.u, info.v);
            return true;
        }
        //std::cout << "命中" << std::endl;
    }
    return false;
}

//求球体的包围盒
bool Sphere::boundingBox(double t0, double t1, AABB& box) const
{
    box = AABB(
        center - Vec3(radius, radius, radius),
        center + Vec3(radius, radius, radius)
    );
    return true;
}

//点坐标转化为(u,v)坐标
void Sphere::getUV(const Vec3& point, double& u, double& v) const
{
    double phi = atan2(point.z(), point.x());
    double theta = asin(point.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}

#endif