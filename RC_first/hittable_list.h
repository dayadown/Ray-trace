#pragma once
//����ײ�б�
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;
//
// shared_ptr<double> double_ptr = make_shared<double>(0.37);
// shared_ptr<vec3>   vec3_ptr = make_shared<vec3>(1.414214, 2.718281, 1.618034);
// shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0);
// make_shared<thing>()  ʹ��thing()������캯������������Ϊthing������ָ��

class hittable_list : public hittable {
public:
    //objects��ſɱ��������������ָ���б�
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }
    //�жϹ����������б����ײ��������շ��ص�ֵ��bool���Ƿ��������
                                             //���ã����е���Ϣrec
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                //��¼���о�������ĵ㣬�����������hit�����У�ֻ�Թ��ߵ�ray_tmin,ray_tmax��Χ���������ж�
                //����������У�˵����ray_tmaxС����ray_tmax�̳����ϸ���������ı��棬������������ÿ�ζ�����
                //������Ǹ����е�
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};