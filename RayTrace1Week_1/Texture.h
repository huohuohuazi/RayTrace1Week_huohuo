#pragma once
#include "stb_image.h"
#include "Vec3.h"

class Texture {
public:
	unsigned char* data;
	int nx, ny;
public:
	Texture(){}
	Texture(unsigned char *pixels,int A,int B):data(pixels),nx(A),ny(B){}
	~Texture() { delete data; }

    virtual Vec3 value(double u, double v, const Vec3& p) const {
        //未找到纹理，使用灰代替
        if (data == nullptr)
            return Vec3(0, 1, 1);

        int i = int((u)*nx);
        int j = int((1 - v) * ny - 0.001);

        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx - 1) i = nx - 1;
        if (j > ny - 1) j = ny - 1;

        auto r = int(data[3 * i + 3 * nx * j + 0]) / 255.0;
        auto g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
        auto b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;

        return Vec3(r, g, b);
    }
};