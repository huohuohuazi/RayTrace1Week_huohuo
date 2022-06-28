#include<iostream>
#include<vector>
#include<fstream>
#include <iomanip>
#include<Windows.h>

#include "Sphere.h"
#include "Utils.h"
#include "Hitlist.h"
#include "Camera.h"
#include "Materials.h"


using namespace std;
#define INF 1000000000

const int image_width = 200;
const int image_height = 100;
//Vec3 img[image_width][image_height];
Vec3 img[image_width* image_height];


void output_ppm(int *p,int height,int width)
{
    //int* img = p;

    ofstream out_txt_file;
    out_txt_file.open("./output.ppm", ios::out | ios::trunc);

    //P3表示rgb图像
    out_txt_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    
    int index = 0;
    for (int j = height-1; j >=0 ; j--) {
        for (int i = 0; i < width; i++) {
            //写入数据
            //out_txt_file << img[i][j].x() << ' ' << img[i][j].y() << ' ' << img[i][j].z() << '\n';
            out_txt_file << int(255 * img[index].x()) << ' ' << int(255 * img[index].y()) << ' ' << int(255 * img[index].z()) << '\n';
        
            index++;
        }
    }
    //关闭文件
    out_txt_file.close();

}

//递归反射
//与物体进行碰撞，深度为depth，以为光线最多反射depth
Vec3 ray_color(const Ray& r,const Hit& world,int depth)
{
    hit_info info;

    //停止循环反射
    if (depth <= 0) { return Vec3(0, 0, 0); }

    /*如果光线与物体相交*/
    if (world.hit(r, 0.001, infinity, info))//0.001防止自交
    {
        Ray ray_out;
        Vec3 attenuation;

        if (info.material->scatter(r, info, attenuation, ray_out))
            return(attenuation * ray_color(ray_out, world, depth - 1));
        //将交点作为新光线的发射点，也许是漫反射的随机光线，也可以是镜面反射的确定光线
        return Vec3(0, 0, 0);

        //大部分的光线都会被吸收, 而不是被反射
        //表面越暗, 吸收就越有可能发生
        
    }

    /*背景色*/
    else
    {
        Vec3 direction = unit_vector(r.direction());
        double t = 0.5 * (direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
        //return Vec3(1, 0, 0);
    }
    
}

int main()
{
    const int samples_per_pixel = 100;//抗锯齿采样次数
    const int max_depth = 50;//最大递归次数

    /*画布，或者说相机*/
    const double aspect = double(image_width) / image_height;
    const double fov = 90;
    const Vec3 vup = Vec3(0,1,0);

    Vec3 look_from(0,1,2);
    Vec3 look_to(0, 1, 1);
    //景深
    double lens_pos = (look_from - look_to).length();//焦距
    double lens_d = 1;//光圈大小

    Camera camera(look_from, look_to, vup, fov, aspect, lens_d, lens_pos);
    //Camera camera;//还是默认位置好（

    /*构造场景*/
    HitList world;

    //金属球
    world.add(make_shared<Sphere>(Vec3(1, 0,-1), -0.5, new Dielec(1.5)));
    world.add(make_shared<Sphere>(Vec3(-1,0,-1), 0.5, new Matal(Vec3(0.8, 0.6, 0.8), 1.0)));
    //漫反射球
    world.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5, new Lambert(Vec3(0.8, 0.3, 0.3))));
    //地面
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100, new Lambert(Vec3(0.8, 0.3, 0.0))));


    /*绘制图像*/
    int index = 0;
    for (int j = image_height-1; j >=0 ; j--) {
        cout << "\r当前进度: " << (1-(double(j)/ double(image_height)))*100.0<<'%' << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                //选取同一像素内，中心周围的区域
                double u = double(i + random_double()) / image_width;
                double v = double(j + random_double()) / image_height;

                /*对于每个光线，获取其颜色*/
                Ray r = camera.getRay(u, v);//在u,v位置发射一个光线
                color += ray_color(r, world, max_depth);
            }

            color /= samples_per_pixel;
            color.SelfLimit();
            color.SeflSqrt();//gamma指数
            img[index] = color;
            index++;

        }
    }
    cout << endl << "done." << endl;


    //保存文件
    int* p;
    p = (int*)img;
    output_ppm(p, image_width, image_height);
}
