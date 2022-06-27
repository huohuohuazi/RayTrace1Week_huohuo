#include<iostream>
#include<vector>
#include<fstream>
#include <iomanip>
#include<Windows.h>

#include "sphere.h"
#include "utils.h"
#include "hitlist.h"



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


//与物体进行碰撞
//当参数为HitList时，将遍历List中的所有物体进行碰撞
Vec3 ray_color(const Ray& r,const Hit& world)
{
    hit_info info;
    /*如果碰到*/
    if (world.hit(r, 0, infinity, info))
    {
        return 0.5 * (info.normal + Vec3(1, 1, 1));
    }

    /*背景色*/
    else
    {
        Vec3 direction = unit_vector(r.direction());
        double t = 0.5 * (direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
    
}

int main()
{
    /*画布*/
    //左下角为起点
    Vec3 coner_left_down(-2.0, -1.0, 1.0);
    //长宽
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    //起点
    Vec3 origin(0.0, 0.0, 0.0);

    /*构造场景*/
    HitList world;
    world.add(make_shared<Sphere>(Vec3(0, 0, 1), 0.5));
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    /*绘制图像*/
    int index = 0;
    for (int j = image_height-1; j >=0 ; j--) {
        cout << "\r当前进度: " << (1-(double(j)/ double(image_height)))*100.0<<'%' << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            //遍历每个像素
            double u = double(i) / image_width;
            double v = double(j) / image_height;
            
            /*对于每个光线，获取其颜色*/
            Ray r(origin, coner_left_down + u * horizontal + v * vertical);
            
            Vec3 color = ray_color(r,world);
            
            img[index] = color;
            index++;
        }
    }
    cout << "done." << endl;


    //保存文件
    int* p;
    p = (int*)img;
    output_ppm(p, image_width, image_height);
}
