#ifndef PERLIN
#define PERLIN
#include "Vec3.h"
#include "utils.h"

//线性插值，使得纹理更加平滑
inline double trilinear_interp(double c[2][2][2], double u, double v, double w) {
    double accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * u + (1 - i) * (1 - u)) *
                (j * v + (1 - j) * (1 - v)) *
                (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accum;
}
inline double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                Vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww))
                    * dot(c[i][j][k], weight_v);
            }

    return accum;
}



//柏林噪声纹理
class Perlin
{
private:
	static const int pointCount = 256;
	Vec3* randomVec;

	int* perm_x;
	int* perm_y;
	int* perm_z;

    //随机交换p*中的元素
    static void permute(int* p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    //初始化p并产生perm？
    static int* perlin_generate_perm() {
        int* p = new int[pointCount];

        for (int i = 0; i < pointCount; i++)  p[i] = i;

        permute(p, pointCount);
        return p;
    }
    

public:
    Perlin() {
        randomVec = new Vec3[pointCount];

        for (int i = 0; i < pointCount; i++) {
            randomVec[i] = unit_vector(random(-1, 1));
        }
        //随机整数数组
        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] randomVec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }
    
    //产生噪声，作为纹理返回给材质
    double noise(const Vec3& p) const {
        //小数部分
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());

        //如果直接使用uvw会产生明显的“格子”的痕迹，是因为线性变化造成的
        //使用hermite cube平滑插值
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        //整数部分
        int i = int(floor(p.x()));
        int j = int(floor(p.y()));
        int k = int(floor(p.z()));

        Vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = randomVec[
                        perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255]
                    ];

        return perlin_interp(c, u, v, w);
    }

    //扰动，或者叫叠加噪声
    double turb(const Vec3& p, int depth = 7) const {
        double accum = 0.0;
        Vec3 temp_p = p;
        double weight = 1.0;
        
        //使用多种噪声叠加
        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

};




#endif
