#ifndef RAY_H_
#define RAY_H_

#include "Vector.h"

class Ray
{
public:
	Ray() : m_origin( Vector( 0, 0, 0 ) ), m_direction( Vector( 0, 0, 0 ) ) {};
	Ray( const Vector& a_origin, const Vector& a_dir ) : m_origin(a_origin), m_direction(a_dir) {};
	
	Vector& Origin() { return m_origin; }
	Vector& Direction() { return m_direction; }
private:
	Vector m_origin;
	Vector m_direction;
};

#endif /*RAY_H_*/
