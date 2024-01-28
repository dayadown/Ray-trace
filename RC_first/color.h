#pragma once
#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    //传入的pixel_color为[0,1]的颜色值
    //samples_per_pixel：每个像素样本数量
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    // 将颜色做平均处理（计算真实贡献）
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    // 转换并输出[0,255]区间的颜色值
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}