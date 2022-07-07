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
#include "MovingSphere.h"
#include "BVH.h"
#include "Rectangle.h"

using namespace std;
#define INF 1000000000

const int image_width = 1000;
const int image_height = 1000;

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
//与物体进行碰撞，深度为depth，以为光线最多反射depth。事实上首先碰撞的是BVH包围盒
Vec3 ray_color(const Ray& ray_in, const Vec3& background,const Hit& world,int depth)
{
    hit_info info;

    //停止循环反射
    if (depth <= 0) { return Vec3(0, 0, 0); }

    /*如果光线与物体相交*/
    if (world.hit(ray_in, 0.001, infinity, info))//0.001防止自交
    {
        Ray ray_out;
        Vec3 attenuation;//折射or反射的方向以及强度

        Vec3 emitted = info.material->emit(info.u, info.v, info.point);//发光信息 

        //如果不反射，则为发光材质
        if (!info.material->scatter(ray_in, info, attenuation, ray_out))
            return emitted;
        //进行反射，将交点作为新光线的发射点，也许是漫反射的随机光线，也可以是镜面反射的确定光线
        return (attenuation * ray_color(ray_out, background, world, depth - 1));
        

        //大部分的光线都会被吸收, 而不是被反射
        //表面越暗, 吸收就越有可能发生
        
    }

    /*背景色*/
    else
    {
        //以前返回蓝色背景，现在去除环境光
        return background;
        //Vec3 direction = unit_vector(r.direction());
        //double t = 0.5 * (direction.y() + 1.0);
        //return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
        //return Vec3(1, 0, 0);
    }
    
}

HitList GenerateWorld()
{
    /*构造场景*/
    HitList world;
    //地面
    /*world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, new Lambert(new Checker_Texture(
        new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
        new ConstantTexture(Vec3(0.9, 0.9, 0.9))
        ))));*/
   //world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, new Lambert((new NoiseTexture(4)))));


    //墙壁材质
    Material* red = new Lambert(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambert(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambert(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
    //灯光材质
    Material* light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));

    
    //墙壁
    //左
    world.add(make_shared<Flipface>(make_shared<YZRectangle>(0, 555, 0, 555, 555, green)));
    //右
    world.add(make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
    
    //上
    world.add(make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
    
    //下
    world.add(make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
    
    //后
    world.add(make_shared<Flipface>(make_shared<XYRectangle>(0, 555, 0, 555, 555, white)));
    

    //灯光
    world.add(make_shared<XZRectangle>(213, 343, 227, 332, 554, light));

    /*随机小球*/
    /*
    int i = 1;
    for (int a = -11; a < 11; a++) {

        break;//不画小球了，太慢了

        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random() * random();
                    world.add(
                        make_shared<Sphere>(center, 0.2, new Lambert(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        make_shared<Sphere>(center, 0.2, new Matal(albedo, fuzz)));
                }
                else {
                    // glass
                    world.add(make_shared<Sphere>(center, 0.2, new Dielec(1.5)));
                }
            }
        }
    }
    */
    
    //将场景生成为BVH二叉树
    //return (BVH_node(world, 0, 1));
    return world;
}




int main()
{
    const int samples_per_pixel = 100;//抗锯齿采样次数
    const int max_depth = 80;//最大递归次数
    const Vec3 background(0, 0, 0);

    /*画布，或者说相机*/
    const double aspect = double(image_width) / image_height;
    const double fov = 40;
    const Vec3 vup = Vec3(0,1,0);

    Vec3 look_from(278, 278, -800);
    Vec3 look_to(278, 278, 0);
    
    double lens_pos = 10.0;//焦距
    double lens_d = 0.0;//光圈大小
    Camera camera(look_from, look_to, vup, fov, aspect, lens_d, lens_pos, 0.0, 1.0);
    //Camera camera;//还是默认位置好（


    //场景
    HitList world = GenerateWorld();
    //BVH_node bvhtree= GenerateWorld();


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
                color += ray_color(r, background, world, max_depth);
                //color += ray_color(r, background, bvhtree, max_depth);
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
