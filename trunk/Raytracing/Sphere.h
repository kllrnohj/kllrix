#ifndef SPHERE_H_
#define SPHERE_H_

#include "Primitive.h"
#include "Vector.h"
#include "Material.h"

class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(const Vector& center, double radius, int isLight = 0)
		: m_center(center), m_radius(radius), m_isLight(isLight) { };
	virtual ~Sphere();
	
	int Intersect(Ray& ray, double& dist);
	int IsLight() { return m_isLight; }
	Vector GetNormal(const Vector& pt);
	Vector& GetCenter() { return m_center; }
private:
	Vector m_center;
	double m_radius;
	int m_isLight;

};

#endif /*SPHERE_H_*/
