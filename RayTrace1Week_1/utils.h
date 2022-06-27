
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

//�ǶȻ���
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}
//�Ƚϴ�С
inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

//�����������
//����0-1֮��������
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
//����min-max֮��������
inline double random_double(double min,double max) {
    return min + (max - min) * random_double();
}
//��x������min,max��
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


// Common Headers

#include "Ray.h"
#include "Vec3.h"

#endif