#pragma once
//#include "stb_image.h"
#include "Vec3.h"

class Texture {
public:
	//Texture(unsigned char *pixels,int A,int B):data(pixels),nx(A),ny(B){}
	//~Texture() { delete data; }

	//�ӿڣ���ȡpoint����������������Ϊ(u,v)
	virtual Vec3 value(double u, double v, const Vec3& point) const = 0;
};


//��ɫ����
class ConstantTexture :public Texture
{
public:
	Vec3 color;
public:
	ConstantTexture(){}
	ConstantTexture(Vec3 c):color(c){}

	virtual Vec3 value(double u, double v, const Vec3& point) const;
};
Vec3 ConstantTexture::value(double u, double v, const Vec3& point) const
{
	return color;
}


//���̲��ʣ�ֻ��������ɫ��������դ�񻯵Ĳ��ʣ��Ǹ�������λ�ö����������
class Checker_Texture : public Texture {
public:
    ConstantTexture *odd;//ż�������
    ConstantTexture *even;
public:
    Checker_Texture() {}
    Checker_Texture(ConstantTexture* t0, ConstantTexture* t1) : even(t0), odd(t1) {}

    virtual Vec3 value(double u, double v, const Vec3& point) const;
};

Vec3 Checker_Texture::value(double u, double v, const Vec3& point) const {
    double sines = sin(10 * point.x()) * sin(10 * point.y()) * sin(10 * point.z());
    if (sines < 0)
        return odd->value(u, v, point);
    
    else
        return even->value(u, v, point);
}