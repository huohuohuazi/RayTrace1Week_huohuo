#ifndef BVH
#define BVH

#include "Utils.h"
#include "Hitlist.h"
#include "AABB.h"
#include <algorithm>

//using namespace std;

static bool box_x_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b);
static bool box_y_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b);
static bool box_z_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b);

//BVH�������Ľڵ�
class BVH_node :public Hit
{
public:
	AABB node_box;
	shared_ptr<Hit> left;//��ڵ�
	shared_ptr<Hit> right;//�ҽڵ�

public:
	BVH_node(){}


    //����ʱ��Ҫ�Ż�����ÿ����һ�����壬����������������б��Ϊ����
	BVH_node(
		std::vector<shared_ptr<Hit>>& objects,//�����б�
		size_t start, size_t end,             //�б������巶Χ
        double time0, double time1) 
	{
        //���ѡȡһ��ά�Ƚ��зָ�
        int axis = random_int(0, 2);

        //��ʼ���˺���ָ��
        auto comparator = &box_x_compare;
        comparator =
            (axis == 0) ? &box_x_compare :
            (axis == 1) ? &box_y_compare :
            &box_z_compare;

        //�����ѡȡ��Χ��Ĭ��Ϊ��0������Ŀ
        size_t object_span = end - start;

        //���ֻ��һ�����壬����˵�ݹ鵽Ҷ�ӽڵ�
        if (object_span == 1) {
            left = right = objects[start];
        }
        //���ֻ���������壬����˵�ݹ鵽��Ҷ�ӽڵ�
        else if (object_span == 2) {
            //��������ķ�����ڵ㣬��������ķ����ҽڵ�
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        //��������������壬�򰴴�С�������м�ڵ㿪ʼ�ݹ��������ҽڵ�
        else {
            //����λ�����������б�
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            //���м�ڵ㿪ʼ�ݹ�������������
            size_t mid = start + object_span / 2;
            //ÿ��make_shared���Ƕ��ӽڵ���ù��캯�����Ӷ�ʵ�ֵݹ�
            left = make_shared<BVH_node>(objects, start, mid, time0, time1);
            right = make_shared<BVH_node>(objects, mid, end, time0, time1);
        }

        //������ȡ�ӽڵ�İ�Χ��
        AABB box_left, box_right;
        if (!left->boundingBox(time0, time1, box_left) ||
            !right->boundingBox(time0, time1, box_right))
            std::cerr << "�����ڵ��û�а�Χ��\n";
        //�Ӷ��������ڵ�İ�Χ��
        node_box = surroundingBox(box_left, box_right);
	}


	BVH_node(HitList list, 
        double time0, double time1) :
		BVH_node(list.objects,      //�����б�
            0, list.objects.size(), //�б������巶Χ
            time0, time1){}

	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
	virtual bool boundingBox(double t0, double t1, AABB& box) const;

    

};



//��ȡ������İ�Χ�У�һ������¶����ڰ�Χ�У�����һЩ�����ͼԪ�����������ͼԪ
bool BVH_node::boundingBox(double t0, double t1, AABB& box) const
{
	box = node_box;
	return true;
}

//�ݹ���ýڵ���ӽڵ��Ƿ���ײ���������ҽڵ���ײ���򷵻���ײ
bool BVH_node::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
	if (!node_box.hit(ray_in, tmin, tmax)) return false;

	bool IsLeftHit = left->hit(ray_in, tmin, tmax, info);
	bool IsRightHit = right->hit(ray_in, tmin, tmax, info);
	
	return IsLeftHit || IsRightHit;
}




//�Ƚ�������Χ����ĳһά���ϵĴ�С����a��bС�򷵻���
bool box_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b, int axis) {
    AABB tempbox_a;
    AABB tempbox_b;

    if (!a->boundingBox(0, 0, tempbox_a) || !b->boundingBox(0, 0, tempbox_b))
        std::cerr << "�����ڵ��û�а�Χ��\n";

    return (tempbox_a.minNum().e[axis] < tempbox_b.minNum().e[axis]);
}

//�Ƚ�������Χ����xά���ϵĴ�С����a��bС�򷵻���
bool box_x_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 0);
}
//�Ƚ�������Χ����yά���ϵĴ�С����a��bС�򷵻���
bool box_y_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 1);
}
//�Ƚ�������Χ����zά���ϵĴ�С����a��bС�򷵻���
bool box_z_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 2);
}


#endif