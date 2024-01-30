#pragma once
//�����
#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include <iostream>
class camera {
public:

    //������Ⱦ�����ͼƬ������
    double aspect_ratio = 1.0;  // Ratio of image width over height����߱�
    int image_width = 100;  // Rendered image width in pixel count����Ⱦ��ͼ��Ŀ�
    int samples_per_pixel = 10;   // Count of random samples for each pixel��ÿ�����ص�������
    int max_depth = 10;   // Maximum number of ray bounces into scene ���ݹ����

    //��Ⱦ�����������ÿ�����ط�����ߣ�����ȡ���ص���ɫ����󽫸���ɫ��Ⱦ��������
    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                //������ѭ����ѭ������ÿ������ÿ��������ֵ�����Ӻ�
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
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


    //��ʼ����������
    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        //�����ӿھ����������
        auto focal_length = 1.0;
        //�����ӿڣ������ӿڵĹ���������һһ��Ӧ
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        //�����ӿ����Ͻ�Ϊ(0,0)���������µ����������ӽ��ƶ�������
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        //��������Ⱦͼ��ĳһ����(u��v)�ƶ�һ������ʱ�������ӿ��ƶ��Ķ������൱��һ���ƶ�������ӳ�䣩
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        //viewport_upper_left�������ָ�������ӿ����Ͻǵ���ά����
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        //pixel00_locָ����������(���ϵ�����)
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }


    /*
    * һ�������������е������ཻ���õ�����ɫ
    */

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;//����hit����ʱΪ���ã���¼���е����Ϣ

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        //����world�ࣨһ���ɱ�������ļ����ࣩ��hit����
        //interval(0.001, infinity)�����Ծ��뵱ǰ��������ر���Ľ��㣬���������ڼ��㾫�ȵĴ����
        if (world.hit(r, interval(0.001, infinity), rec)) {

            // �����ͽ��㷨��ͬһ�ߵ��������
            // vec3 direction = random_on_hemisphere(rec.normal);

            // �Ż��������һ������ĵ�λ�����õ���������һ���뷨��ͬһ�ߵ�����
            vec3 direction = rec.normal + random_unit_vector();
            //�ݹ������䣬�ݹ�10�����ڣ����صݹ���ɫ��50%(��������)
            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }


        //��û�л��У�����Ⱦ����ɫ���������ɫ��
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }


    /*
    * ��ȡ��i��j�������ڵ������������
    */
    ray get_ray(int i, int j) const {

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    /*
    * ��ȡ����������Ϊ��������ƶ�����
    */
    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        // px,py��Ϊ[-0.5,0.5]
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        // ���ӿ��������Ų����������ӿ���һ�����صĶ���Ϊpixel_delta_u,pixel_delta_v�����ǵ�λֵ1
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
