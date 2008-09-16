#ifndef VECTOR_H_
#define VECTOR_H_

class Vector
{
protected:
	double x, y, z;
	
public:
	Vector();
	Vector(const Vector &copy);
	Vector(double x, double y, double z);
	virtual ~Vector();
	
	// operator overloads
	Vector & operator=(const Vector &rhs);
	Vector & operator+=(const Vector &rhs);
	const Vector operator+(const Vector &other) const;
	Vector & operator-=(const Vector &rhs);
	const Vector operator-(const Vector &other) const;
	const bool operator==(const Vector &other) const;
	const bool operator!=(const Vector &other) const;
	
	// scalar
	Vector & operator*=(const double rhs);
	const Vector operator*(const double other) const;
	
	// other vector stuff
	double magnitude();
	const Vector normalize(double len = 1) const;
	
	double& X() { return x; }
	double& Y() { return y; }
	double& Z() { return z; }
	
	double DotProduct(const Vector &other);
};

#endif /*VECTOR_H_*/
