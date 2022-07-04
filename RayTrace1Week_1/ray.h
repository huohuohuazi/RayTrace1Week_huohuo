#pragma once
#ifndef RAY
#define RAY
#include "Vec3.h"

class Ray {
private:
	Vec3 ori;
	Vec3 dir;
	double tm;

public:

	Ray() {}
	Ray(const Vec3& origin,const Vec3& direction,double time=0.0):ori(origin),dir(direction),tm(time) {}

	Vec3 origin() const { return ori; }
	Vec3 direction() const { return (dir); }
	double time() const { return tm; }

	Vec3 at(double t) const {
		return ori + t * dir;
	}

};

#endif // !RAY
