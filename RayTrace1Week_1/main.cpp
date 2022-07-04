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

    //P3��ʾrgbͼ��
    out_txt_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    
    int index = 0;
    for (int j = height-1; j >=0 ; j--) {
        for (int i = 0; i < width; i++) {
            //д������
            //out_txt_file << img[i][j].x() << ' ' << img[i][j].y() << ' ' << img[i][j].z() << '\n';
            out_txt_file << int(255 * img[index].x()) << ' ' << int(255 * img[index].y()) << ' ' << int(255 * img[index].z()) << '\n';
        
            index++;
        }
    }
    //�ر��ļ�
    out_txt_file.close();

}

//�ݹ鷴��
//�����������ײ�����Ϊdepth����Ϊ������෴��depth
Vec3 ray_color(const Ray& r,const Hit& world,int depth)
{
    hit_info info;

    //ֹͣѭ������
    if (depth <= 0) { return Vec3(0, 0, 0); }

    /*��������������ཻ*/
    if (world.hit(r, 0.001, infinity, info))//0.001��ֹ�Խ�
    {
        Ray ray_out;
        Vec3 attenuation;//����or����ķ����Լ�ǿ��

        if (info.material->scatter(r, info, attenuation, ray_out))
            return(attenuation * ray_color(ray_out, world, depth - 1));
        //��������Ϊ�¹��ߵķ���㣬Ҳ�����������������ߣ�Ҳ�����Ǿ��淴���ȷ������
        return Vec3(0, 0, 0);

        //�󲿷ֵĹ��߶��ᱻ����, �����Ǳ�����
        //����Խ��, ���վ�Խ�п��ܷ���
        
    }

    /*����ɫ*/
    else
    {
        Vec3 direction = unit_vector(r.direction());
        double t = 0.5 * (direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
        //return Vec3(1, 0, 0);
    }
    
}

HitList GenerateWorld()
{
    /*���쳡��*/
    HitList world;
    //����
    world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, new Lambert(Vec3(0.5, 0.5, 0.5))));
    

    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, new Dielec(1.5)));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, new Matal(Vec3(0.7, 0.6, 0.5), 0.0)));
    
    //world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, new Lambert(Vec3(0.4, 0.2, 0.1))));

    Vec3 center(4, 1, 0);
    world.add(make_shared<Moving_sphere>(
        0.5, center, center + Vec3(0, 1, 0),
        0.0, 1.0, new Lambert(Vec3(0.4, 0.2, 0.1))));




    /*���С��*/
    int i = 1;
    for (int a = -11; a < 11; a++) {

        break;//����С���ˣ�̫����

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
    

    

    return world;
}


int main()
{
    const int samples_per_pixel = 100;//����ݲ�������
    const int max_depth = 50;//���ݹ����

    /*����������˵���*/
    const double aspect = double(image_width) / image_height;
    const double fov = 20;
    const Vec3 vup = Vec3(0,1,0);

    Vec3 look_from(13, 2, 3);
    Vec3 look_to(0, 2,0);
    //����
    double lens_pos = 10.0;//����
    double lens_d = 0.0;//��Ȧ��С

    Camera camera(look_from, look_to, vup, fov, aspect, lens_d, lens_pos,0.0,1.0);
    //Camera camera;//����Ĭ��λ�úã�

    //����
    HitList world = GenerateWorld();


    /*����ͼ��*/
    int index = 0;
    for (int j = image_height-1; j >=0 ; j--) {
        cout << "\r��ǰ����: " << (1-(double(j)/ double(image_height)))*100.0<<'%' << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                //ѡȡͬһ�����ڣ�������Χ������
                double u = double(i + random_double()) / image_width;
                double v = double(j + random_double()) / image_height;

                /*����ÿ�����ߣ���ȡ����ɫ*/
                Ray r = camera.getRay(u, v);//��u,vλ�÷���һ������
                color += ray_color(r, world, max_depth);
            }

            color /= samples_per_pixel;
            color.SelfLimit();
            color.SeflSqrt();//gammaָ��
            img[index] = color;
            index++;

        }
    }
    cout << endl << "done." << endl;


    //�����ļ�
    int* p;
    p = (int*)img;
    output_ppm(p, image_width, image_height);
}
