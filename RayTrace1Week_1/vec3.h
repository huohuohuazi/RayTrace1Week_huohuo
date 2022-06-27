#pragma once
#include <iostream>
//Vec3�࣬������������
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

    //-
    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    //����
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
    //ģ
    double length() const {
        return sqrt(length_squared());
    }
    //ģ��ƽ��
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    //�����
    void write_color(std::ostream& out) {
        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(255.999 * e[0]) << ' '
            << static_cast<int>(255.999 * e[1]) << ' '
            << static_cast<int>(255.999 * e[2]) << '\n';
    }
};

//������
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
//��˳���
inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
//��˳���
inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}
//�������
inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}
//�������
inline double dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}
//���
inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
//��һ��
inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}