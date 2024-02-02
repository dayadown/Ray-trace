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


// 金属表面
class metal : public material {
public:
    metal(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};