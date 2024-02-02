#pragma once
#include "rtweekend.h"
// ���ʳ����࣬����ɢ������
#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};


// ��ģ��ɢ��ͷ����ʣ��ֲڱ��棩
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    // ͨ�������÷���
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        // �������ɢ�䷽��
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        // ���ز�����ɢ�䷽���ά�Ⱦ�Ϊ0�����
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};


// ��������
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