#pragma once
#include "rtweekend.h"
// 材质抽象类，产生散射射线
#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};


// 建模光散射和反射率（粗糙表面）
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    // 通过传引用返回
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        // 产生随机散射方向
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        // 拦截产生的散射方向各维度均为0的情况
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};


// 金属表面,加入模糊fuzz(镜面反射随机微小扰动的效果)
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_unit_vector());//加随机扰动
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};