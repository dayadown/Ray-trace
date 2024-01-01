#pragma once
//可碰撞列表
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;
//
// shared_ptr<double> double_ptr = make_shared<double>(0.37);
// shared_ptr<vec3>   vec3_ptr = make_shared<vec3>(1.414214, 2.718281, 1.618034);
// shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0);
// make_shared<thing>()  使用thing()这个构造函数，返回类型为thing的智能指针

class hittable_list : public hittable {
public:
    //objects存放可被击中物体的智能指针列表
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }
    //判断光线与物体列表的碰撞结果，最终返回的值：bool：是否击中物体
                                             //引用：击中的信息rec
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                //记录击中距离最近的点，由于在物体的hit函数中，只对光线的ray_tmin,ray_tmax范围内做击中判断
                //所以如果击中，说明比ray_tmax小，而ray_tmax继承于上个击中物体的表面，这样就做到了每次都保存
                //最近的那个击中点
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};