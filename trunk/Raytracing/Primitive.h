#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"

class Primitive
{
public:	
	virtual int Intersect(Ray& ray, double& dist) = 0;
	virtual int IsLight() = 0;
	virtual Vector GetNormal(const Vector& pt) = 0;
	Material& GetMaterial() { return m_material; }
private:
	Material m_material;
};

#endif /*PRIMITIVE_H_*/
