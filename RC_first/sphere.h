#pragma once
#include"hittable.h"
//�����࣬�̳г���Ļ����࣬��ʾ���Ա����߻���
class sphere : public hittable {
public:
    sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        //�ж��������ķ�Χ�����ж�С�ģ������ڹ��߷�Χ�ڣ������ڿɽ��ܵ�t��Χ�ڣ��������ײ�㣩
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        //rec.normal = (rec.p - center) / radius;
        //�Ľ����жϷ��ߵĳ��򣬣�set_face_normal��������������ߴ��ⲿ��������߷���Ϊoutward_normal���������
        //���ߴ����ڲ���������棬���߷���Ϊ-outward_normal
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }

private:
    point3 center;
    double radius;
};