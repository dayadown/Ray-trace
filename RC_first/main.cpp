#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>


/*
* 1.0,只固定光线的颜色
*/
//获取光线的颜色 v1.0
/*
color ray_color(const ray& r) {
    //return color(0, 0, 0);
    //渐变色
    vec3 unit_direction = unit_vector(r.direction());//标准化，标准化之后y的范围为（-1,1）
    auto a = 0.5 * (unit_direction.y() + 1.0);//a根据y的值计算为（0,1）的范围
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);//渐变效果，根据a的值分配白色与蓝色的权重
}
*/


/*
* 加入球，判断光线是否与球相交，相交即返回红色
*/

/*
//判断光线是否与球相交
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

//获取光线的颜色 v2.0
color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
*/


//写死只渲染一个固定的球
/*
* 根据交点的法线渲染
*/
/*
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0;
    }
    else {
        //返回较小的解，即近的曲面交点
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        //N为法向，从圆心指向交点，且单位化
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
*/

/*
* 一根光线与世界中的物体相交而得到的颜色
*/

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;//传入hit函数时为引用，记录击中点的信息
    //调用world类（一个可被击中类的集合类）的hit函数
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }


    //若没有击中，则渲染背景色（渐变的蓝色）
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


/*
* 简化版的判断交点的函数,化简去掉b的2倍
*/
/*
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}
*/


int main() {

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World
    
    //定义世界
    hittable_list world;

    //世界中加入可被击中的物体
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera


    //虚拟视口距离相机距离
    auto focal_length = 1.0;
    //虚拟视口，虚拟视口的光线与像素一一对应
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    //虚拟视口左上角为(0,0)，往右往下递增
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    //计算在渲染图沿某一方向(u或v)移动一个像素时，虚拟视口移动的度量（相当于一个移动度量的映射）
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    //viewport_upper_left即从相机指向虚拟视口左上角的三维向量
    auto viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    //pixel00_loc指向像素中心(左上的像素)
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            //每次移动一个像素
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}