#pragma once
//相机类
#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include <iostream>
class camera {
public:

    //定义渲染结果的图片的属性
    double aspect_ratio = 1.0;  // Ratio of image width over height，宽高比
    int    image_width = 100;  // Rendered image width in pixel count，渲染的图像的宽

    //渲染函数，相机对每个像素发射光线，并获取返回的颜色，最后将该颜色渲染在像素上
    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below


    //初始化各个参数
    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        //虚拟视口距离相机距离
        auto focal_length = 1.0;
        //虚拟视口，虚拟视口的光线与像素一一对应
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        //虚拟视口左上角为(0,0)，往右往下递增，计算视角移动的向量
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        //计算在渲染图沿某一方向(u或v)移动一个像素时，虚拟视口移动的度量（相当于一个移动度量的映射）
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        //viewport_upper_left即从相机指向虚拟视口左上角的三维向量
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        //pixel00_loc指向像素中心(左上的像素)
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }


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
};
