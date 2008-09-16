#ifndef PLANE_H_
#define PLANE_H_

#include "Vector.h"
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane();
	Plane(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& v4);
	
	Vector & operator[](int i);
	double getWidth();
	double getHeight();
	double getZAt(double x, double y);
	
	int Intersect(Ray& ray, double& dist);
	int IsLight() { return 0; }
	Vector GetNormal(const Vector& pt) { return pt; }
private:
	Vector m_corners[4];
};

#endif /*PLANE_H_*/
