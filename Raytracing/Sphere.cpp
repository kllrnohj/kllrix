#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

int Sphere::Intersect(Ray& ray, double& dist)
{
	// no intersecting with light :)
	if (m_isLight) return 0;
	
	Vector el = m_center - ray.Origin();
	double d = el.DotProduct(ray.Direction());
	double els = el.DotProduct(el);
	double rs = m_radius * m_radius;
	
	if (d < 0 && els > rs)
	{
		return 0;
	}
	
	double ms = els - d*d;
	
	if (ms > rs)
	{
		return 0;
	}
	
	double q = sqrt(rs - ms);
	
	if (els > rs)
	{
		dist = d - q;
	} else {
		dist = d + q;
	}
	
	return 1;
}

Vector Sphere::GetNormal(const Vector& pt)
{
	return pt - m_center;
}
