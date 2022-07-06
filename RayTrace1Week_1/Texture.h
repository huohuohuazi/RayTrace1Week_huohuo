#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Vec3.h"
#include "Perlin.h"

//纹理类（纹理是材质的一部分）
class Texture {
public:
	//Texture(unsigned char *pixels,int A,int B):data(pixels),nx(A),ny(B){}
	//~Texture() { delete data; }

	//接口，获取point处的纹理，纹理坐标为(u,v)
	virtual Vec3 value(double u, double v, const Vec3& point) const = 0;
};




//纯色纹理
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




//棋盘纹理，只有两种颜色，但不是栅格化的材质，是跟随物体位置而计算出来的
class Checker_Texture : public Texture {
public:
    ConstantTexture *odd;//偶数格材质
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




//噪声纹理
class NoiseTexture :public Texture
{
public:
	Perlin noise;//噪声行为
	double scale;//噪声频率
public:
	NoiseTexture():scale(1) {}
	NoiseTexture(double sc) :scale(sc) {}

	virtual Vec3 value(double u, double v, const Vec3& point) const;
};
Vec3 NoiseTexture::value(double u, double v, const Vec3& point) const
{
	//将结果映射到0-1之间
	
	//使用单一噪声
	//return Vec3(1, 1, 1) * 0.5 * (1 + noise.noise(scale * point));
	//return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale * point.z() + 10 * noise.noise(point)));

	//使用扰动or叠加噪声，同时利用sin函数调整相位，使得条纹起伏
	return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale * point.z() + 10 * noise.turb(point)));
}


//图片纹理
class ImageTexture :public Texture
{
public:
	unsigned char* data;//图片数组（rgb三维展成一维）
	int nx, ny, nn;//图像长、宽、通道数
public:
	ImageTexture() {}
	//通过图片名称读取图像，并保存到纹理中
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
	//没有图片数据
	if (data == nullptr)
	{
		std::cout << "No Image Source" << std::endl;
		return Vec3(0, 0, 1);
	}
		

	//(u,v)坐标映射为图片中的(i,j)
	int i = int((u)*nx);
	int j = int((1 - v) * ny - 0.001);

	//限制范围
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





