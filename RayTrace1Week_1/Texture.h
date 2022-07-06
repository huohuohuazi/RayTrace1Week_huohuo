#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Vec3.h"
#include "Perlin.h"

//�����ࣨ�����ǲ��ʵ�һ���֣�
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




//��������ֻ��������ɫ��������դ�񻯵Ĳ��ʣ��Ǹ�������λ�ö����������
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




//��������
class NoiseTexture :public Texture
{
public:
	Perlin noise;//������Ϊ
	double scale;//����Ƶ��
public:
	NoiseTexture():scale(1) {}
	NoiseTexture(double sc) :scale(sc) {}

	virtual Vec3 value(double u, double v, const Vec3& point) const;
};
Vec3 NoiseTexture::value(double u, double v, const Vec3& point) const
{
	//�����ӳ�䵽0-1֮��
	
	//ʹ�õ�һ����
	//return Vec3(1, 1, 1) * 0.5 * (1 + noise.noise(scale * point));
	//return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale * point.z() + 10 * noise.noise(point)));

	//ʹ���Ŷ�or����������ͬʱ����sin����������λ��ʹ���������
	return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale * point.z() + 10 * noise.turb(point)));
}


//ͼƬ����
class ImageTexture :public Texture
{
public:
	unsigned char* data;//ͼƬ���飨rgb��άչ��һά��
	int nx, ny, nn;//ͼ�񳤡���ͨ����
public:
	ImageTexture() {}
	//ͨ��ͼƬ���ƶ�ȡͼ�񣬲����浽������
	ImageTexture(const char* imagename)
	{
		data= stbi_load(imagename, &nx, &ny, &nn, 0);
		//std::cout << nx <<"," << ny<<"," << nn << std::endl;
		//std::cout << data << std::endl;
	}

	~ImageTexture() { delete data; }

	virtual Vec3 value(double u, double v, const Vec3& point) const;
};
Vec3 ImageTexture::value(double u, double v, const Vec3& point) const
{
	//û��ͼƬ����
	if (data == nullptr)
	{
		std::cout << "No Image Source" << std::endl;
		return Vec3(0, 0, 1);
	}
		

	//(u,v)����ӳ��ΪͼƬ�е�(i,j)
	int i = int((u)*nx);
	int j = int((1 - v) * ny - 0.001);

	//���Ʒ�Χ
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;

	//static_cast<int>
	double r = ((data[3 * i + 3 * nx * j + 0])	/	255.0);
	double g = ((data[3 * i + 3 * nx * j + 1])  /	255.0);
	double b = ((data[3 * i + 3 * nx * j + 2])	/	255.0);

	//std::cout << r << "\n" << g << "\n" << b << "\n" << std::flush;

	return Vec3(r, g, b);
}





