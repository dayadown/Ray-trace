#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <iostream>
int main() {
    // World
    
    //定义世界
    hittable_list world;

    //世界中加入可被击中的物体
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;
    cam.aspect_ratio = 16 / 9;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);
}