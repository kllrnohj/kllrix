#include "Vector.h"
#include <cmath>

Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::~Vector()
{
}

Vector::Vector(const Vector & copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector& Vector::operator=(const Vector& rhs)
{
	// check for self-assignment
	if (this == &rhs)
		return *this;
	
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	
	return *this;
}

Vector& Vector::operator +=(const Vector& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	
	return *this;
}

const Vector Vector::operator+(const Vector &other) const
{
	return Vector(*this) += other;
}

Vector& Vector::operator -=(const Vector& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	
	return *this;
}

const Vector Vector::operator-(const Vector &other) const
{
	return Vector(*this) -= other;
}

const bool Vector::operator==(const Vector &other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

const bool Vector::operator!=(const Vector &other) const
{
	return !(*this == other);
}

Vector & Vector::operator*=(const double rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	
	return *this;
}

const Vector Vector::operator*(const double other) const
{
	return Vector(*this) *= other;
}

double Vector::magnitude()
{
	return sqrt(x*x + y*y + z*z);
}

const Vector Vector::normalize(double len) const
{
	return Vector(*this) *= len/sqrt(x*x + y*y + z*z);
}

double Vector::DotProduct(const Vector &other)
{
	return x * other.x + y * other.y + z * other.z;
}
