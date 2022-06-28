#pragma once
#include <iostream>
#include "Utils.h"
//Vec3类，用于向量操作
class Vec3 {
public:
    double e[3];
public:
    Vec3() :e{ 0,0,0 } {}
    Vec3(double e0, double e1, double e2) :e{ e0,e1,e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    void SelfLimit()
    {
        e[0] = e[0] > 1.0 ? 1.0 : e[0];
        e[1] = e[1] > 1.0 ? 1.0 : e[1];
        e[2] = e[2] > 1.0 ? 1.0 : e[2];

        e[0] = e[0] < 0.0 ? 0.0 : e[0];
        e[1] = e[1] < 0.0 ? 0.0 : e[1];
        e[2] = e[2] < 0.0 ? 0.0 : e[2];   
    }
    //开根号
    void SeflSqrt() {
        e[0] = sqrt(e[0]);
        e[1] = sqrt(e[1]);
        e[2] = sqrt(e[2]);
    }

    //-
    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    //索引
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    //+=
    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    //*=
    Vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    /* /=  */
    Vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }
    //模
    double length() const {
        return sqrt(length_squared());
    }
    //模的平方
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    


    //输出流
    void write_color(std::ostream& out) {
        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(255.999 * e[0]) << ' '
            << static_cast<int>(255.999 * e[1]) << ' '
            << static_cast<int>(255.999 * e[2]) << '\n';
    }

    
    inline static Vec3 random();
    inline static Vec3 random(double min, double max);
    inline double dot(const Vec3& u, const Vec3& v);

};

//输入流
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
//+
inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
//-
inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
//
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
//点乘常数
inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
//点乘常数
inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}
//点除常数
inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}
//点乘向量
inline double dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}
//叉乘
inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
//归一化
inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

//生成随机向量
inline static Vec3 random() {
    return Vec3(random_double(), random_double(), random_double());
}

inline static Vec3 random(double min,double max) {
    return Vec3(random_double(min,max), random_double(min, max), random_double(min, max));
}


//随机选取方向，以前的方法，在外接方格内随机选取点，直至在求内部
inline Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 p = random(-1, 1);
        //寻找到一个在球内的随机点
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

//现在的方法，通过cos函数，使得光线散射更接近分布律
inline Vec3 random_unit_vector()
{
    double a = random_double(0, 2 * pi);//与法线夹角
    double z = random_double(-1,1);//线高度（z轴投影）
    double r = sqrt(1 - z * z);
    //极坐标换算
    return Vec3(r * cos(a), r * sin(a), z);
}

//从单位小圆盘中射出光线，模拟透镜，依然是排除法
inline Vec3 random_in_unit_disk()
{
    while (true)
    {
        Vec3 p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        //寻找到一个在球内的随机点
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

//已知入射光线，求反射光线
inline Vec3 reflect(const Vec3& ray_in, const Vec3& n)
{
    return (ray_in - 2 * dot(ray_in, n) * n);
}

//已知入射光线，求折射光线
inline Vec3 refract(const Vec3& ray_in, const Vec3& n, double etai_over_etat)
{
    auto cos_theta = dot(-ray_in, n);
    Vec3 r_out_parallel = etai_over_etat * (ray_in + cos_theta * n);
    Vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
    return r_out_parallel + r_out_perp;
}