#pragma once

//定义常用的常量，方法
#include <cmath>
#include <limits>
#include <cstdlib>
#include <memory>
#include <random>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();//定义无限大的
const double pi = 3.1415926535897932385;//定义派的大小

// Utility Functions
//根据角计算弧度制
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    // 返回[0,1)之间的随机值
    return rand() / (RAND_MAX + 1.0);
}


inline double random_double_new() {
    //新特性，需要"random"头，返回[0, 1)之间的随机值
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    // 返回[min,max)之间的随机值
    return min + (max - min) * random_double_new();
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"