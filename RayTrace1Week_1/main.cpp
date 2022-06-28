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
        Vec3 attenuation;

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

int main()
{
    const int samples_per_pixel = 100;//����ݲ�������
    const int max_depth = 50;//���ݹ����

    /*����������˵���*/
    const double aspect = double(image_width) / image_height;
    const double fov = 90;
    const Vec3 vup = Vec3(0,1,0);

    Vec3 look_from(3,3,2);
    Vec3 look_to(0,0,-1);
    //����
    double lens_pos = (look_from - look_to).length();//����
    double lens_d = 2;//��Ȧ��С

    Camera camera(look_from, look_to, vup, fov, aspect, lens_d, lens_pos);
    //Camera camera;//����Ĭ��λ�úã�

    /*���쳡��*/
    HitList world;

    //������
    world.add(make_shared<Sphere>(Vec3(1,0, -1),-0.5,new Dielec(1.5)));
    world.add(make_shared<Sphere>(Vec3(-1, 0, -1),0.5,new Matal(Vec3(0.8, 0.6, 0.8),1.0)));
    //��������
    world.add(make_shared<Sphere>(Vec3(0, 0, -1),0.5,new Lambert(Vec3(0.8, 0.3, 0.3))));
    //����
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1),100,new Lambert(Vec3(0.8, 0.3, 0.0))));


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
