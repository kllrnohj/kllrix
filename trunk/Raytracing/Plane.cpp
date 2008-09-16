#include "Plane.h"
#include <stdexcept>

Plane::Plane()
{
	m_corners[0] = Vector(-2,1.5,0);
	m_corners[1] = Vector(2,1.5,0);
	m_corners[2] = Vector(2,-1.5,0);
	m_corners[3] = Vector(-2,-1.5,0);
}

Plane::Plane(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& v4)
{
	m_corners[0] = v1;
	m_corners[1] = v2;
	m_corners[2] = v3;
	m_corners[3] = v4;
}

Vector & Plane::operator[](int i)
{
	if (i < 0 || i >= 4)
		throw std::range_error("Index out of range!");
	return m_corners[i];
}

double Plane::getWidth()
{
	Vector top = m_corners[0] - m_corners[1];
	Vector bot = m_corners[2] - m_corners[3];
	return (top.magnitude() + bot.magnitude()) / 2.0;
}

double Plane::getHeight()
{
	Vector left = m_corners[0] - m_corners[3];
	Vector right = m_corners[1] - m_corners[2];
	return (left.magnitude() + right.magnitude()) / 2.0;
}

double Plane::getZAt(double x, double y)
{
	// since plane doesn't tilt yet, just return the Z of the first corner
	return m_corners[0].Z();
}

int Plane::Intersect(Ray& ray, double& dist)
{
	return 0;
}
