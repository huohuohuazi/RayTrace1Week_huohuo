#pragma once
#ifndef RECTANGLE
#define RECTANGLE

#include "hit.h"


class XYRectangle :public Hit
{
public:
	Material* material;
	double x0, x1, y0, y1, z0, z1;
	double k;//假设是水平的，z只取一个

public:
    XYRectangle(){}
    XYRectangle(double _x0, double _x1, double _y0, double _y1, double _k, Material* _mat)
		:x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), z0(k), z1(k), material(_mat) {};

	
    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
	
    //包围盒是个平面
    virtual bool boundingBox(double t0, double t1, AABB& box) const
	{
		box = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
		return true;
	}


};
bool XYRectangle::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
    double t = (k - ray_in.origin().z()) / ray_in.direction().z();
    //如果超时
    if (t < tmin || t > tmax)  
        return false;

    double x = ray_in.origin().x() + t * ray_in.direction().x();
    double y = ray_in.origin().y() + t * ray_in.direction().y();

    //如果超出矩形范围
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    //交点信息
    info.u = (x - x0) / (x1 - x0);
    info.v = (y - y0) / (y1 - y0);
    info.t = t;

    Vec3 outward_normal = Vec3(0, 0, 1);
    info.set_face_nornal(ray_in, outward_normal);
    info.material = material;
    info.point = ray_in.at(t);
    return true;
}


class XZRectangle :public Hit
{
public:
    Material* material;
    double x0, x1, y0, y1, z0, z1;
    double k;//y只取一个

public:
    XZRectangle() {}
    XZRectangle(double _x0, double _x1, double _z0, double _z1, double _k, Material* _mat)
        :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), material(_mat) {};


    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;

    //包围盒是个平面
    virtual bool boundingBox(double t0, double t1, AABB& box) const
    {
        box = AABB(Vec3(x0, k - 0.0001, z0), Vec3(x1, k - 0.0001, z1));
        return true;
    }


};
bool XZRectangle::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
    double t = (k - ray_in.origin().y()) / ray_in.direction().y();
    //如果超时
    if (t < tmin || t > tmax)
        return false;

    double x = ray_in.origin().x() + t * ray_in.direction().x();
    double z = ray_in.origin().z() + t * ray_in.direction().z();

    //如果超出矩形范围
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    //交点信息
    info.u = (x - x0) / (x1 - x0);
    info.v = (z - z0) / (z1 - z0);
    info.t = t;

    Vec3 outward_normal = Vec3(0, 1, 0);
    info.set_face_nornal(ray_in, outward_normal);
    info.material = material;
    info.point = ray_in.at(t);
    return true;
}

class YZRectangle :public Hit
{
public:
    Material* material;
    double x0, x1, y0, y1, z0, z1;
    double k;//y只取一个

public:
    YZRectangle() {}
    YZRectangle(double _y0, double _y1, double _z0, double _z1, double _k, Material* _mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), material(_mat) {};


    virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;

    //包围盒是个平面
    virtual bool boundingBox(double t0, double t1, AABB& box) const
    {
        box = AABB(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
        return true;
    }


};
bool YZRectangle::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
    double t = (k - ray_in.origin().x()) / ray_in.direction().x();
    //如果超时
    if (t < tmin || t > tmax)
        return false;

    double y = ray_in.origin().x() + t * ray_in.direction().y();
    double z = ray_in.origin().z() + t * ray_in.direction().z();

    //如果超出矩形范围
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    //交点信息
    info.u = (y - y0) / (y1 - y0);
    info.v = (z - z0) / (z1 - z0);
    info.t = t;

    Vec3 outward_normal = Vec3(1, 0, 0);
    info.set_face_nornal(ray_in, outward_normal);
    info.material = material;
    info.point = ray_in.at(t);
    return true;
}




#endif