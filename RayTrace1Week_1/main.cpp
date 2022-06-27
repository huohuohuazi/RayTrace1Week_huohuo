#include<iostream>
#include<vector>
#include<fstream>
#include <iomanip>
#include<Windows.h>

#include "sphere.h"
#include "utils.h"
#include "hitlist.h"
#include "camera.h"


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


//�����������ײ
//������ΪHitListʱ��������List�е��������������ײ
Vec3 ray_color(const Ray& r,const Hit& world)
{
    hit_info info;
    /*�������*/
    if (world.hit(r, 0, infinity, info))
    {
        return 0.5 * (info.normal + Vec3(1, 1, 1));
    }

    /*����ɫ*/
    else
    {
        Vec3 direction = unit_vector(r.direction());
        double t = 0.5 * (direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
    
}

int main()
{
    const int samples_per_pixel = 100;//����ݲ�������


    /*����������˵���*/
    Camera camera;
    //���½�Ϊ���
    Vec3 coner_left_down(-2.0, -1.0, 1.0);
    //����
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    //���
    Vec3 origin(0.0, 0.0, 0.0);

    /*���쳡��*/
    HitList world;
    world.add(make_shared<Sphere>(Vec3(0, 0, 1), 0.5));
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

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
                Ray r(origin, coner_left_down + u * horizontal + v * vertical);
                color += ray_color(r, world);
            }
            color /= samples_per_pixel;
            color.SelfLimit();
            
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
