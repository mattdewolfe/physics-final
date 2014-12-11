#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "Entity.h"
#include "Component.h"

enum GraphicColor { COLOR_Red = 0, COLOR_Blue, COLOR_Green, COLOR_Yellow, COLOR_Count };

class GraphicsComponent : public Component
{
public:
	enum GraphicShapeTypes { GST_Plane = 0, GST_Sphere, GST_Cube };

	GraphicsComponent(GraphicShapeTypes _t) :
		Component(CT_GRAPHIC), 
		type(_t) 
	{ 

	}

	virtual ~GraphicsComponent() {}
	virtual void Init(float _w, float _h) 
	{
		m_Width = _w;
		m_Height = _h;
		m_Radius = 0.5f;
	}
	virtual void Shutdown() {}

	virtual void Update(float deltaTime);
	virtual void Render();

	double GetRadius() const { return m_Radius;}
	void SetRadius(double radius) {m_Radius = radius;}

	float GetHeight() const { return m_Height;}
	void SetHeight(float height) {m_Height = height;}

	float GetWidth() const { return m_Width;}
	void SetWidth(float width) {m_Width = width;}

	void SetColor(GraphicColor _color) { currentColor = _color; ToggleColor(); }
	void SetColor(float _r, float _g, float _b)
	{
		color.x = _r;
		color.y = _g;
		color.z = _b;
	}
	GraphicColor GetColor() { return currentColor; }

	// Switch the color of this graphic
	void ToggleColor();
	void DrawPlane();
	void DrawSphere();
	void DrawCube();

private:
	Entity::EVector3f color;
	GraphicColor currentColor;
	GraphicShapeTypes type;
	double m_Radius;
	float m_Height;
	float m_Width;

protected:
};

#endif