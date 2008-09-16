#ifndef MATERIAL_H_
#define MATERIAL_H_

struct Color
{
	unsigned int R, G, B, A;
	Color() {}
	Color(int r, int g, int b, int a = 0xFF) : R(r), G(g), B(b), A(a) {} 
};

class Material
{
public:
	Material() {}
	Material(Color c, double dif = 1, double spec = 0)
		: m_color(c), m_diffuse(dif), m_specular(spec) {}
	virtual ~Material();
	Color GetColor() { return m_color; }
	double GetDiffuse() { return m_diffuse; }
	double GetSpecular() { return m_specular; }
	void SetColor(Color c) { m_color = c; }
	void SetDiffuse(double dif) { m_diffuse = dif; }
	void SetSpecular(double spec) { m_specular = spec; }
private:
	Color m_color;
	double m_diffuse;
	double m_specular;
};

#endif /*MATERIAL_H_*/
