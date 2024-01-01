#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>


/*
* 1.0,ֻ�̶����ߵ���ɫ
*/
//��ȡ���ߵ���ɫ v1.0
/*
color ray_color(const ray& r) {
    //return color(0, 0, 0);
    //����ɫ
    vec3 unit_direction = unit_vector(r.direction());//��׼������׼��֮��y�ķ�ΧΪ��-1,1��
    auto a = 0.5 * (unit_direction.y() + 1.0);//a����y��ֵ����Ϊ��0,1���ķ�Χ
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);//����Ч��������a��ֵ�����ɫ����ɫ��Ȩ��
}
*/


/*
* �������жϹ����Ƿ������ཻ���ཻ�����غ�ɫ
*/

/*
//�жϹ����Ƿ������ཻ
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

//��ȡ���ߵ���ɫ v2.0
color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
*/


//д��ֻ��Ⱦһ���̶�����
/*
* ���ݽ���ķ�����Ⱦ
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
        //���ؽ�С�Ľ⣬���������潻��
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        //NΪ���򣬴�Բ��ָ�򽻵㣬�ҵ�λ��
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
*/

/*
* һ�������������е������ཻ���õ�����ɫ
*/

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;//����hit����ʱΪ���ã���¼���е����Ϣ
    //����world�ࣨһ���ɱ�������ļ����ࣩ��hit����
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }


    //��û�л��У�����Ⱦ����ɫ���������ɫ��
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


/*
* �򻯰���жϽ���ĺ���,����ȥ��b��2��
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
    
    //��������
    hittable_list world;

    //�����м���ɱ����е�����
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera


    //�����ӿھ����������
    auto focal_length = 1.0;
    //�����ӿڣ������ӿڵĹ���������һһ��Ӧ
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    //�����ӿ����Ͻ�Ϊ(0,0)���������µ���
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    //��������Ⱦͼ��ĳһ����(u��v)�ƶ�һ������ʱ�������ӿ��ƶ��Ķ������൱��һ���ƶ�������ӳ�䣩
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    //viewport_upper_left�������ָ�������ӿ����Ͻǵ���ά����
    auto viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    //pixel00_locָ����������(���ϵ�����)
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            //ÿ���ƶ�һ������
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}