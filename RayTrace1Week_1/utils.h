
#ifndef UTILS
#define UTILS

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>


using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//角度换算
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}
//比较大小
inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

//随机数生成器
//返回0-1之间的随机数
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
//返回min-max之间的随机数
inline double random_double(double min,double max) {
    return min + (max - min) * random_double();
}
//将x限制在min,max中
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


// Common Headers

#include "Ray.h"
#include "Vec3.h"

#endif