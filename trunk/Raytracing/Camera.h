#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector.h"
#include "Plane.h"

class Camera
{
public:
	Camera() : m_origin(Vector(0,0,-1)), m_window(Plane()) {};
	Camera(Vector origin, Plane window) : m_origin(origin), m_window(window) {};

	Vector& getOrigin() { return m_origin; }
	Plane& getWindow() { return m_window; }
private:
	Vector m_origin;
	Plane m_window;
};

#endif /*CAMERA_H_*/
