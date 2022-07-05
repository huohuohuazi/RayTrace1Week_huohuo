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

//BVH二叉树的节点
class BVH_node :public Hit
{
public:
	AABB node_box;
	shared_ptr<Hit> left;//左节点
	shared_ptr<Hit> right;//右节点

public:
	BVH_node(){}


    //构建时需要优化树，每加入一个物体，都会沿着轴把物体列表分为两半
	BVH_node(
		std::vector<shared_ptr<Hit>>& objects,//物体列表
		size_t start, size_t end,             //列表中物体范围
        double time0, double time1) 
	{
        //随机选取一个维度进行分割
        int axis = random_int(0, 2);

        //初始化了函数指针
        auto comparator = &box_x_compare;
        comparator =
            (axis == 0) ? &box_x_compare :
            (axis == 1) ? &box_y_compare :
            &box_z_compare;

        //物体的选取范围，默认为从0到总数目
        size_t object_span = end - start;

        //如果只有一个物体，或者说递归到叶子节点
        if (object_span == 1) {
            left = right = objects[start];
        }
        //如果只有两个物体，或者说递归到了叶子节点
        else if (object_span == 2) {
            //靠近负轴的放在左节点，靠近正轴的放在右节点
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        //如果大于两个物体，则按大小排序后从中间节点开始递归设置左右节点
        else {
            //根据位置排序物体列表
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            //从中间节点开始递归设置左右子树
            size_t mid = start + object_span / 2;
            //每次make_shared都是对子节点调用构造函数，从而实现递归
            left = make_shared<BVH_node>(objects, start, mid, time0, time1);
            right = make_shared<BVH_node>(objects, mid, end, time0, time1);
        }

        //迭代获取子节点的包围盒
        AABB box_left, box_right;
        if (!left->boundingBox(time0, time1, box_left) ||
            !right->boundingBox(time0, time1, box_right))
            std::cerr << "两个节点均没有包围盒\n";
        //从而获得自身节点的包围盒
        node_box = surroundingBox(box_left, box_right);
	}


	BVH_node(HitList list, 
        double time0, double time1) :
		BVH_node(list.objects,      //物体列表
            0, list.objects.size(), //列表中物体范围
            time0, time1){}

	virtual bool hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const;
	virtual bool boundingBox(double t0, double t1, AABB& box) const;

    

};



//获取该物体的包围盒，一般情况下都存在包围盒，除了一些特殊的图元如无限延伸的图元
bool BVH_node::boundingBox(double t0, double t1, AABB& box) const
{
	box = node_box;
	return true;
}

//递归检查该节点的子节点是否碰撞，如果左或右节点碰撞到则返回碰撞
bool BVH_node::hit(const Ray& ray_in, double tmin, double tmax, hit_info& info) const
{
	if (!node_box.hit(ray_in, tmin, tmax)) return false;

	bool IsLeftHit = left->hit(ray_in, tmin, tmax, info);
	bool IsRightHit = right->hit(ray_in, tmin, tmax, info);
	
	return IsLeftHit || IsRightHit;
}




//比较两个包围盒在某一维度上的大小，若a比b小则返回真
bool box_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b, int axis) {
    AABB tempbox_a;
    AABB tempbox_b;

    if (!a->boundingBox(0, 0, tempbox_a) || !b->boundingBox(0, 0, tempbox_b))
        std::cerr << "两个节点均没有包围盒\n";

    return (tempbox_a.minNum().e[axis] < tempbox_b.minNum().e[axis]);
}

//比较两个包围盒在x维度上的大小，若a比b小则返回真
bool box_x_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 0);
}
//比较两个包围盒在y维度上的大小，若a比b小则返回真
bool box_y_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 1);
}
//比较两个包围盒在z维度上的大小，若a比b小则返回真
bool box_z_compare(const shared_ptr<Hit> a, const shared_ptr<Hit> b) {
    return box_compare(a, b, 2);
}


#endif