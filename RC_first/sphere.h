#pragma once
#include"hittable.h"
//球体类，继承抽象的击中类，表示可以被光线击中
class sphere : public hittable {
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center(_center), radius(_radius), mat(_material) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        //判断两个根的范围（先判断小的）必须在光线范围内（查找在可接受的t范围内，最近的碰撞点）
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        //rec.normal = (rec.p - center) / radius;
        //改进：判断法线的朝向，（set_face_normal函数）：如果光线从外部打进球，则法线方向为outward_normal，否则如果
        //光线从球内部打向球表面，则法线方向为-outward_normal
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat; // 定义材质
};